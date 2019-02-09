#pragma once

#include <EventSystem/Event.h>
#include <Objects/Agents/FedoraAgent.h>

namespace Fed
{
	// When an agent (human or ai) throws a fedora. Called from input component
	// Both agents and fedora recieves this
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

	// Picked fedora from the floor or wild
	// Called from collision
	class FrisbeePickupEvent : public Event
	{
	public:
		FrisbeePickupEvent(Vector3 pickupPosition, const FedoraAgent& agent)
			: m_PickupPosition(pickupPosition), m_Agent(agent)
		{
		}

		inline Vector3 GetPosition() const { return m_PickupPosition; }
		inline const FedoraAgent& GetAgent() const { return m_Agent; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "FrisbeePickup from Agent " << m_Agent.GetID();
			return ss.str();
		}

		EVENT_CLASS_TYPE(FrisbeePickup)
		EVENT_CLASS_CATEGORY(EventCategoryFrisbeeField)
	private:
		Vector3 m_PickupPosition;
		const FedoraAgent& m_Agent;
	};
}