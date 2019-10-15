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
		FrisbeePickupEvent(Vector3 pickupPosition, FedoraAgent& agent)
			: m_PickupPosition(pickupPosition), m_Agent(agent)
		{
		}

		inline Vector3 GetPosition() const { return m_PickupPosition; }
		inline FedoraAgent& GetAgent() const { return m_Agent; }

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
		FedoraAgent& m_Agent;
	};

	// Scored in a goal
	// Called from collision
	class FrisbeeScoredEvent : public Event
	{
	public:
		FrisbeeScoredEvent(TeamColor scoringTeam, const FedoraAgent* scoringAgent)
			: m_ScoringTeam(scoringTeam), m_ScoringAgent(scoringAgent)
		{
		}

		inline TeamColor GetScoringTeam() const { return m_ScoringTeam; }
		inline const FedoraAgent* GetAgent() const { return m_ScoringAgent; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Frisbee Scored from Agent " << m_ScoringAgent->GetID();
			return ss.str();
		}

		EVENT_CLASS_TYPE(FrisbeeScored)
		EVENT_CLASS_CATEGORY(EventCategoryFrisbeeField)
	private:
		TeamColor m_ScoringTeam;
		const FedoraAgent* m_ScoringAgent;
	};

	// Game ended
	class GameEndedEvent : public Event
	{
	public:
		GameEndedEvent(int blueScore, int redScore)
			: m_BlueScore(blueScore), m_RedScore(redScore)
		{
		}

		inline std::pair<int, int> GetScores() const { return std::pair<int, int>(m_BlueScore, m_RedScore); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Game ended!";
			return ss.str();
		}

		EVENT_CLASS_TYPE(GameEnded)
		EVENT_CLASS_CATEGORY(EventCategoryFrisbeeField)
	private:
		int m_BlueScore;
		int m_RedScore;
	};
}