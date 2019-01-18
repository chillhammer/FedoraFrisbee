#include "FedPCH.h"
#include "GameManager.h"
#include "GameStates.h"
#include <GLFW/glfw3.h>
#include "GameManager.h"

namespace Fed
{
	GameManager& GameManager::Instance()
	{
		static GameManager instance;
		return instance;
	}

	void GameManager::Init()
	{
	}
	static WindowProps props;
	GameManager::GameManager() : m_StateMachine(this, Test::Instance()), MainCamera(),
		m_Window(props)
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

	float GameManager::DeltaTime() const
	{
		return m_DeltaTime;
	}

	GLFWwindow * GameManager::GetWindowPtr() const
	{
		return m_Window.m_Window;
	}

	// Delta time allows objects to move despite rendering lag
	void GameManager::UpdateDeltaTime()
	{
		double currentTime = glfwGetTime();
		m_DeltaTime = currentTime - m_LastUpdatedTime;
		m_LastUpdatedTime = currentTime;
	}

}