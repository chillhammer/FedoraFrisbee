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

	// Attempt to get fedora from agent
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
}