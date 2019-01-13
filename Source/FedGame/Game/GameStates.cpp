#include <FedPCH.h>
#include "GameStates.h"
/**
	Each state runs code that determines the course of the game.
**/
namespace Fed
{
	// Test State
	void Test::Enter(Game* owner)
	{
		LOG("Entering Test State in Game Object");
	}
	void Test::Execute(Game* owner)
	{
	}
	void Test::Exit(Game* owner)
	{
	}
}