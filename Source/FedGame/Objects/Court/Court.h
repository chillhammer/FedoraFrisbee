#pragma once
#include <GameObject/GameObject.h>
#include <StateMachine/StateMachine.h>
#include <EventSystem/IObserver.h>
#include <EventSystem/Events/FrisbeeFieldEvent.h>

namespace Fed
{
	struct GoalTrigger
	{
		Team* TeamOwner;
		GameObject Object;
	};
	/**
		Court scenery. Background
	**/
	class Court : public GameObject
	{
	public:
		Court();

		const std::vector<GameObject>& GetWalls() const;
		std::vector<const GameObject*> GetCollidingWalls(const GameObject& other) const;
		void DrawDebugWalls() const;
	private:
		std::vector<GameObject> m_Walls;
		std::vector<GoalTrigger> m_Goals;
	};
}