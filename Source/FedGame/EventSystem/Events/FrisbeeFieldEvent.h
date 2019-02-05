#pragma once

#include <EventSystem/Event.h>
#include <Objects/Agents/FedoraAgent.h>

namespace Fed
{
	class FrisbeeThrownEvent : public Event
	{
	public:
		FrisbeeThrownEvent(Vector3 thrownDirection, Vector3 thrownPosition, const FedoraAgent& agent)
			: m_ThrownDirection(thrownDirection), m_ThrownPosition(thrownPosition), m_Agent(agent)
		{
		}

		inline Vector3 GetDirection() const { return m_ThrownDirection; }
		inline Vector3 GetPosition() const { return m_ThrownPosition; }
		inline const FedoraAgent& GetAgent() const { return m_Agent; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "FrisbeeThrown";
			return ss.str();
		}

		EVENT_CLASS_TYPE(FrisbeeThrown)
		EVENT_CLASS_CATEGORY(EventCategoryFrisbeeField)
	private:
		Vector3 m_ThrownDirection;
		Vector3 m_ThrownPosition;
		const FedoraAgent& m_Agent;
	};
}