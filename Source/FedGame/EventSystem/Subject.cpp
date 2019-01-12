#include <FedPCH.h>
#include "Subject.h"
#include "IObserver.h"

namespace Fed
{
	void Subject::AddObserver(IObserver* observer)
	{
		m_Observers.push_back(observer);
	}

	void Subject::RemoveObserver(IObserver* observer)
	{
		m_Observers.remove(observer);
	}

	void Subject::Notify(Event& event) const
	{
		for (IObserver* observer : m_Observers)
		{
			observer->OnEvent(this, event);
		}
	}
}