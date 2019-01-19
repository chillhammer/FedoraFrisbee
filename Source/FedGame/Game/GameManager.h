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
		const Window& GetWindow() const;
		~GameManager();
	private:
		GameManager();
		void UpdateDeltaTime();

		// Properties
		double m_LastUpdatedTime = 0;
		double m_DeltaTime = 0;

		// Big Classes - in Initialized Order
		Window m_Window;
	public:
		Camera MainCamera;
	private:
		StateMachine<GameManager> m_StateMachine;
	};
}