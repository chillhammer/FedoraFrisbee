#pragma once
#include <FedPCH.h>
#include "IObserver.h"

namespace Fed
{
	// Holds list of listeners to activate upon event
	class Subject
	{
	protected:
		std::list<class IObserver*>	m_Observers;

	public:
		void AddObserver(class IObserver* observer);
		void RemoveObserver(class IObserver* observer);
		void Notify(class Event* const event);
	};
}