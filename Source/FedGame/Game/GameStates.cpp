#include <FedPCH.h>
#include "GameStates.h"
/**
	Each state runs code that determines the course of the game.
**/
namespace Fed
{
	// Test State
	void Test::Enter(GameManager* owner)
	{
		LOG("Entering Test State in Game Object");
		owner->MainCamera.Init();
	}
	void Test::Execute(GameManager* owner)
	{
		owner->MainCamera.Update();
	}
	void Test::Exit(GameManager* owner)
	{
	}
}