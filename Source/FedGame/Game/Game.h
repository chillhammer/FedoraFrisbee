#pragma once
#include <StateMachine/StateMachine.h>
/**
	Controller object handling what happens when.
	Mostly dealing with in-game logic.
**/
namespace Fed
{
	class Game
	{
	public:
		Game();
		~Game();
		void Run();

	private:
		void UpdateDeltaTime();

		// Properties
		StateMachine<Game> m_StateMachine;
		double m_LastUpdatedTime = 0;
		double m_DeltaTime = 0;
	};
}