#pragma once

#include <EventSystem/Event.h>
#include <Objects/Agents/FedoraAgent.h>

namespace Fed
{
	class WaitSignal : public Event 
	{
	public: 
		WaitSignal() {}
		std::string ToString() const override {
			return "Wait Signal";
		}
		EVENT_CLASS_TYPE(Wait)
		EVENT_CLASS_CATEGORY(EventCategoryTeam)
	};

	class PursueSignal : public Event
	{
	public:
		PursueSignal() {}
		std::string ToString() const override {
			return "Pursue Signal";
		}
		EVENT_CLASS_TYPE(Pursue)
		EVENT_CLASS_CATEGORY(EventCategoryTeam)
	};

	// Attempt to get fedora into goal
	class ScoreSignal : public Event
	{
	public:
		ScoreSignal() {}
		std::string ToString() const override {
			return "Score Signal";
		}
		EVENT_CLASS_TYPE(Score)
		EVENT_CLASS_CATEGORY(EventCategoryTeam)
	};

	// guards post until enemy comes near
	class DefendSignal : public Event
	{
	public:
		DefendSignal() {}
		std::string ToString() const override {
			return "Defend Signal";
		}
		EVENT_CLASS_TYPE(Defend)
		EVENT_CLASS_CATEGORY(EventCategoryTeam)
	};

	// Attempt to get fedora from agent
	class StealSignal : public Event
	{
	public:
		StealSignal(FedoraAgent& agent) : m_Agent(agent) {}
		std::string ToString() const override {
			return "Steal Signal";
		}
		inline FedoraAgent& GetAgent() const { return m_Agent; }

		EVENT_CLASS_TYPE(Steal)
		EVENT_CLASS_CATEGORY(EventCategoryTeam)
	private:
		FedoraAgent& m_Agent;
	};

	// Attempt to get fedora from agent
	class StunSignal : public Event
	{
	public:
		StunSignal(float time) : m_Time(time) {}
		std::string ToString() const override {
			return "Stun Signal";
		}
		inline float GetTime() const { return m_Time; }

		EVENT_CLASS_TYPE(Stun)
		EVENT_CLASS_CATEGORY(EventCategoryTeam)
	private:
		float m_Time;
	};
}