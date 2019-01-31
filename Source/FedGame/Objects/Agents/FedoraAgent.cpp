#include <FedPCH.h>
#include <Resources/ResourceManager.h>
#include "FedoraAgent.h"

namespace Fed
{
	FedoraAgent::FedoraAgent()
		: GameObject("RoboPadron")
	{
	}
	FedoraAgent::FedoraAgent(Vector3 position)
		: GameObject("RoboPadron", position)
	{
	}
}
