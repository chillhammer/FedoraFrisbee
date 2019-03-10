#pragma once
#include <vector>
#include <StateMachine/StateMachine.h>

namespace Fed
{
	enum TeamColor {Red, Blue};
	// Team controller. Overwatch AI that controls players
	class Team
	{
	public:
		Team();
		TeamColor	GetColor() const;
		void		SetColor(TeamColor color);
		bool HasAgent(int agentID) const;
		void AddAgent(class FedoraAgent* agent);
		void RemoveAgent(class FedoraAgent* agent);
	private:
		StateMachine<Team> m_StateMachine;
		std::vector<class FedoraAgent*> m_Agents;
		TeamColor m_Color;
		class FedoraAgent* m_ControllingAgent;
	};
}