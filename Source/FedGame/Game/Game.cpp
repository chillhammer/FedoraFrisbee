#include "FedPCH.h"
#include "Game.h"
#include <GLFW/glfw3.h>

namespace Fed
{



	Game::Game() : m_StateMachine(this)
	{
	}

	Game::~Game()
	{
	}

	void Game::Run()
	{
		UpdateDeltaTime();
		m_StateMachine.Update();
	}

	/**
		Delta time allows objects to move despite rendering lag
	**/
	void Game::UpdateDeltaTime()
	{
		double currentTime = glfwGetTime();
		m_DeltaTime = currentTime - m_LastUpdatedTime;
		m_LastUpdatedTime = currentTime;
	}

}