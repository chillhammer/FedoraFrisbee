#include "FedPCH.h"
#include "Game.h"
#include "GameStates.h"
#include <GLFW/glfw3.h>

namespace Fed
{
	Game::Game() : m_StateMachine(this, Test::Instance())
	{
	}

	Game::~Game()
	{
	}

	// Main game loop. State Machine contains in-game logic
	void Game::Run()
	{
		UpdateDeltaTime();
		m_StateMachine.Update();
	}

	// Delta time allows objects to move despite rendering lag
	void Game::UpdateDeltaTime()
	{
		double currentTime = glfwGetTime();
		m_DeltaTime = currentTime - m_LastUpdatedTime;
		m_LastUpdatedTime = currentTime;
	}

}