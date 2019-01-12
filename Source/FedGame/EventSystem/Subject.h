#pragma once
#include <FedPCH.h>
#include "IObserver.h"

namespace Fed
{
	// Holds list of listeners to activate upon event. T is derived Event
	class Subject
	{
	protected:
		std::list<class IObserver*>	m_Observers;

	public:
		Subject() {};
		void AddObserver(class IObserver* observer);
		void RemoveObserver(class IObserver* observer);
		void Notify(class Event& event) const;
	};

}