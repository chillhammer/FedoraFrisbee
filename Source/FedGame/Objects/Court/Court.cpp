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
		for (int i = 0; i < 6; i++)
			m_Walls.emplace_back();
		m_Walls[0].SetBoundingBox(Vector3(10.0f, -5.0f, 10.f), Vector3(10.0f, 10.f, 5.0f));
	}

	// Get walls vector
	const std::vector<GameObject>& Court::GetWalls() const
	{
		return m_Walls;
	}

	// Get Wall that an object is colliding with
	// May cause dangling pointer, do not store this pointer
	GameObject* Court::GetCollidingWall(const GameObject& other) const
	{
		for (GameObject wall : m_Walls)
		{
			if (wall.IsColliding(other))
				return &wall;
		}
		return nullptr;
	}

	// Draws walls' debug box
	void Court::DrawDebugWalls() const
	{
		for (GameObject wall : m_Walls)
		{
			wall.DrawBoundingBox();
		}
	}
	
}