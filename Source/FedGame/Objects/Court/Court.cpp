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
		m_Walls.reserve(6);
		for (int i = 0; i < 6; i++)
			m_Walls.emplace_back();

		m_Walls[0].SetBoundingBox(Vector3(15.f, 1.0f, 41.f), Vector3(6.f, 1.0f, 5.f));	// Blue left wall
		m_Walls[1].SetBoundingBox(Vector3(-15.f, 1.0f, 41.f), Vector3(6.f, 1.0f, 5.f));	// Blue right wall
		m_Walls[2].SetBoundingBox(Vector3( 0.f, 1.0f, 46.5f), Vector3(8.f, 1.0f, 5.f));	// Blue black wall
	}

	// Get walls vector
	const std::vector<GameObject>& Court::GetWalls() const
	{
		return m_Walls;
	}

	// Get Wall that an object is colliding with
	// May cause dangling pointer, do not store this pointer
	const GameObject* Court::GetCollidingWall(const GameObject& other) const
	{
		int i = 0;
		for (const GameObject& wall : m_Walls)
		{
			if (wall.IsColliding(other))
			{
				std::vector<GameObject>::const_iterator it = m_Walls.begin() + i;
				return &(*it);
			}
			++i;
		}
		return nullptr;
	}

	// Draws walls' debug box
	void Court::DrawDebugWalls() const
	{
		for (const GameObject& wall : m_Walls)
		{
			wall.DrawBoundingBox();
		}
	}
	
}