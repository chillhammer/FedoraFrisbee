#include <FedPCH.h>
#include "Subject.h"
#include "IObserver.h"

namespace Fed
{
	void Subject::AddObserver(IObserver* const observer)
	{
		// No Duplicates, May Optimize This
		if (std::find(m_Observers.begin(), m_Observers.end(), observer) == m_Observers.end())
			m_Observers.push_back(observer);
	}

	void Subject::RemoveObserver(IObserver* const observer)
	{
		m_Observers.remove(observer);
	}

	void Subject::Notify(Event& event) const
	{
		for (IObserver* observer : m_Observers)
		{
			// TODO: Maybe stop loop if event is Handled
			observer->OnEvent(this, event);
		}
	}
}