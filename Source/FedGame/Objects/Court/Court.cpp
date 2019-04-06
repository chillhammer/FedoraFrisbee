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
		m_Walls.reserve(8);
		for (int i = 0; i < 8; i++)
			m_Walls.emplace_back();

		m_Walls[0].SetBoundingBox(Vector3(15.f, 1.0f, 36.f), Vector3(6.f, 1.0f, 5.f));	// Blue left wall
		m_Walls[1].SetBoundingBox(Vector3(-15.f, 1.0f, 36.f), Vector3(6.f, 1.0f, 5.f));	// Blue right wall
		m_Walls[2].SetBoundingBox(Vector3( 0.f, 1.0f, 42.5f), Vector3(9.f, 1.0f, 5.f));	// Blue black wall

		m_Walls[3].SetBoundingBox(Vector3(21.25f, 1.0f, 0.f), Vector3(0.5f, 1.0f, 40.f));	// Left side wall
		m_Walls[4].SetBoundingBox(Vector3(-21.25f, 1.0f, 0.f), Vector3(0.5f, 1.0f, 40.f));	// Right side wall

		m_Walls[5].SetBoundingBox(Vector3(15.f, 1.0f, -36.f), Vector3(6.f, 1.0f, 5.f));	// Red left wall
		m_Walls[6].SetBoundingBox(Vector3(-15.f, 1.0f, -36.f), Vector3(6.f, 1.0f, 5.f));	// Red right wall
		m_Walls[7].SetBoundingBox(Vector3(0.f, 1.0f, -42.5f), Vector3(9.f, 1.0f, 5.f));	// Red black wall
	}

	// Get walls vector
	const std::vector<GameObject>& Court::GetWalls() const
	{
		return m_Walls;
	}

	// Get Wall that an object is colliding with
	// May cause dangling pointer, do not store this pointer
	std::vector<const GameObject*> Court::GetCollidingWalls(const GameObject& other) const
	{
		std::vector<const GameObject*> walls;
		walls.reserve(2);
		int i = 0;
		for (const GameObject& wall : m_Walls)
		{
			if (wall.IsColliding(other))
			{
				std::vector<GameObject>::const_iterator it = m_Walls.begin() + i;
				walls.push_back(&(*it));
			}
			++i;
		}
		return walls;
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