#include <FedPCH.h>
#include "States/GameStates.h"
#include <GLFW/glfw3.h>
#include <EventSystem/Event.h>
#include <Input/InputManager.h>
#include <EventSystem/Events/KeyEvent.h>
#include "GameManager.h"

namespace Fed
{
	GameManager& GameManager::Instance()
	{
		static GameManager instance;
		return instance;
	}

	// Called to setup dependencies, namely GLFW
	void GameManager::Init()
	{
		Input.KeyPressed.AddObserver(this);
	}
	// Called to enter first state
	void GameManager::Start()
	{
		m_StateMachine.ChangeState(GameStates::TeamMatch::Instance());
	}
	static WindowProps props;
	GameManager::GameManager() : m_Window(props),
		m_StateMachine(this)
	{
	}

	GameManager::~GameManager()
	{
	}

	// Main game loop. State Machine contains in-game logic
	void GameManager::Run()
	{
		UpdateDeltaTime();
		m_StateMachine.Update();

		// Poll Window Events
		m_Window.OnUpdate();
	}

	bool GameManager::IsRunning() const
	{
		return m_Running;
	}

	void GameManager::Shutdown()
	{
		LOG("Shutting down game");
		m_Running = false;
	}

	void GameManager::ChangeState(State<GameManager>* state)
	{
		m_StateMachine.ChangeState(state);
	}

	void GameManager::OnEvent(const Subject * subject, Event & event)
	{
		Evnt::Dispatch<KeyPressedEvent>(event, EVENT_BIND_FN(GameManager, OnKeyPressed));
	}


	// Returns delta time with scale
	// Will return 0 if paused
	float GameManager::DeltaTime() const
	{
		// Pause / Glitch
		if (m_DeltaTime > 1) {
			return m_TimeScale;
		}
		return m_DeltaTime * m_TimeScale;
	}

	// Returns delta time ignoring time scale, eg. pause
	// Useful for debug methods
	float GameManager::DeltaTimeUnscaled() const
	{
		return m_DeltaTime;
	}

	float GameManager::TimeScale() const
	{
		return m_TimeScale;
	}

	void GameManager::SetTimeScale(float timeScale)
	{
		m_UnpausedTimeScale = timeScale;
		m_TimeScale = timeScale;
	}

	void GameManager::SetTimeScaleFreeze(bool freeze)
	{
		if (!freeze)
			m_TimeScale = m_UnpausedTimeScale;
		else if (m_TimeScale != 0.0f)
		{
			m_UnpausedTimeScale = m_TimeScale;
			m_TimeScale = 0.0f;
		}
	}

	void GameManager::SetPause(bool pause)
	{
		m_Paused = pause;
		SetTimeScaleFreeze(pause);
	}

	bool GameManager::IsPaused() const
	{
		return m_Paused;
	}

	const Window& GameManager::GetWindow() const
	{
		return m_Window;
	}

	// Delta time allows objects to move despite rendering lag
	void GameManager::UpdateDeltaTime()
	{
		double currentTime = glfwGetTime();
		m_DeltaTime = glm::min(currentTime - m_LastUpdatedTime, 1.0);
		m_LastUpdatedTime = currentTime;
	}

	bool GameManager::OnKeyPressed(KeyPressedEvent& e)
	{
		if (IsPaused())
			return false;

		switch (e.GetKeyCode())
		{
		case KEY_X:
			SetTimeScaleFreeze(m_TimeScale != 0.0f);
			break;
		case KEY_R:
			m_StateMachine.ChangeState(GameStates::TeamMatch::Instance());
			break;
		}
		return false;
	}

}