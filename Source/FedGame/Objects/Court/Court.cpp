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
		//SetBoundingBox(Vector3(0, 0, 0.35f), Vector3(0.4, 0.25f, 0.4));
	}
	
}