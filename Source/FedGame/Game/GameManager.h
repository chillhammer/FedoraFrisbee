#pragma once
#include <StateMachine/StateMachine.h>
#include <Camera/Camera.h>
#include <Window/Window.h>
#define Game GameManager::Instance()
namespace Fed
{
	/**
		Manager singleton object handling the game logic.
		Mostly dealing with in-game logic.
	**/
	class GameManager : IObserver
	{
	public:
		static GameManager& Instance();
		void Init();
		void Start();
		void Run();
		bool IsRunning() const;
		void Shutdown();
		void OnEvent(const Subject* subject, Event& event);

		float DeltaTime() const;
		float DeltaTimeUnscaled() const;
		float TimeScale() const;
		void SetTimeScale(float timeScale);
		void TogglePause();
		bool IsPaused() const;

		const Window& GetWindow() const;
		~GameManager();
	private:
		GameManager();
		void UpdateDeltaTime();
		bool OnKeyPressed(class KeyPressedEvent& e);

		// Properties
		double m_LastUpdatedTime = 0;
		double m_DeltaTime = 0;
		double m_TimeScale = 1;
		double m_UnpausedTimeScale = 1;
		bool m_Running = true;

		// Big Classes - in Initialized Order
		Window m_Window;
	private:
		StateMachine<GameManager> m_StateMachine;
	};
}