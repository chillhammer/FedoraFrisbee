#include <FedPCH.h>
#include "Subject.h"
#include "IObserver.h"

namespace Fed
{
	void Subject::AddObserver(IObserver * const observer)
	{
		m_Observers.push_back(observer);
	}

	void Subject::RemoveObserver(IObserver * const observer)
	{
		m_Observers.remove(observer);
	}

	void Subject::Notify(Event * const event)
	{
		for (IObserver* const observer : m_Observers)
		{
			observer->OnEvent(this, event);
		}
	}
}