#pragma once
#include <GameObject/GameObject.h>
#include <StateMachine/StateMachine.h>
#include <EventSystem/IObserver.h>
#include <EventSystem/Events/FrisbeeFieldEvent.h>

namespace Fed
{
	struct GoalTrigger
	{
		TeamColor Color;
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
		const std::vector<GoalTrigger>& GetGoals() const;
		std::vector<const GameObject*> GetCollidingWalls(const GameObject& other) const;
		const GoalTrigger* GetCollidingGoal(const GameObject& other) const;
		Vector3 GetGoalPosition(const Team* team) const;
		bool IsPointWithinField(Vector3 point) const;
		bool IsPointWithinCorner(Vector3 point) const;
		void DrawDebugWalls() const;
		void DrawDebugGoals() const;
	private:
		std::vector<GameObject> m_Walls;
		std::vector<GoalTrigger> m_Goals;
	};
}