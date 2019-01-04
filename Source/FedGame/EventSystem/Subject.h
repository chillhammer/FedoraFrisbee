#pragma once
#include <FedPCH.h>
#include "IObserver.h"

namespace Fed
{
	// Holds list of listeners to activate upon event
	class Subject
	{
	protected:
		std::list<class IObserver* const>	m_Observers;

	public:
		void AddObserver(class IObserver* const observer);
		void RemoveObserver(class IObserver* const observer);
		void Notify(class Event* const event);
	};
}