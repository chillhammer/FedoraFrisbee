#pragma once
#include <StateMachine/StateMachine.h>
#include <Camera/Camera.h>
#include <Window/Window.h>
/**
	Manager singleton object handling the game logic.
	Mostly dealing with in-game logic.
**/
#define Game GameManager::Instance()
namespace Fed
{
	class GameManager
	{
	public:
		static GameManager& Instance();
		void Init();
		void Run();
		float DeltaTime() const;
		GLFWwindow* GetWindowPtr() const;
		~GameManager();
	public:
		Camera MainCamera;
	private:
		GameManager();
		void UpdateDeltaTime();

		// Properties
		StateMachine<GameManager> m_StateMachine;
		double m_LastUpdatedTime = 0;
		double m_DeltaTime = 0;
		Window m_Window;

	};
}