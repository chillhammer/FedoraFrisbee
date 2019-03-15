#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include <Game/GameManager.h>
#include <Resources/ResourceManager.h>
#include "Court.h"

namespace Fed
{

	Court::Court() 
		:	GameObject("Court")
	{
	}

	bool Court::IsColliding(const GameObject & other) const
	{
		for (int i = 0; i < NUM_BOUNDING_BOXES; ++i)
		{
			// TODO: Replace all bounding boxes with an array
		}
		return false;
	}
	
}