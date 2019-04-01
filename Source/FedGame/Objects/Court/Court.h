#pragma once
#include <GameObject/GameObject.h>
#include <StateMachine/StateMachine.h>
#include <EventSystem/IObserver.h>
#include <EventSystem/Events/FrisbeeFieldEvent.h>

namespace Fed
{
	/**
		Court scenery. Background
	**/
	class Court : public GameObject
	{
	public:
		Court();

		const std::vector<GameObject>& GetWalls() const;
		GameObject* GetCollidingWall(const GameObject& other) const;
		void DrawDebugWalls() const;
	private:
		// TODO: Group of wall objects for collision
		std::vector<GameObject> m_Walls;
	};
}