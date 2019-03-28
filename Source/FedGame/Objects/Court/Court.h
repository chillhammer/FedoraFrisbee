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
		bool IsColliding(const GameObject& other) const override;
	private:
		
	};
}