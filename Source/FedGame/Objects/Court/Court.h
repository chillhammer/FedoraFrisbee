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
		static const int NUM_BOUNDING_BOXES = 6;
	private:
		BoundingBox m_BoundingBoxes[NUM_BOUNDING_BOXES];
		
	};
}