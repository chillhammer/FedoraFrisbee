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

	bool Court::IsColliding(const GameObject & other) const
	{
		for (int i = 0; i < NUM_BOUNDING_BOXES; ++i)
		{
			// if (m_BoundingBoxes[i].IsIntersecting(ObjectTransform, other.ObjectTransform, other.m_BoundingBox)
			// TODO: Replace all bounding boxes with an array
		}
		return false;
	}
	
}