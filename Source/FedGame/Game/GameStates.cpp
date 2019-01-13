#include <FedPCH.h>
#include "GameStates.h"
namespace Fed
{
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