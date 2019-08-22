#pragma once
#include <vector>
#include <StateMachine/StateMachine.h>

namespace Fed
{
	enum TeamColor {Red, Blue};
	enum TeamPlay {Offensive, Defensive, None};
	// Team controller. Overwatch AI that controls players
	class Team
	{
	public:
		Team();
		TeamPlay	GetPlay() const;
		TeamColor	GetColor() const;
		void		SetColor(TeamColor color);
		void BroadcastSignal(class Event& event);
		bool HasAgent(int agentID) const;
		void AddAgent(class FedoraAgent* agent);
		void Update();
		void RemoveAgent(class FedoraAgent* agent);
		void ResetPositions();
		void ExitStandoff();
		void SetTeamPlay(TeamPlay play, class FedoraAgent* agentWithFedora);
		void SetFieldControllerReference(class FrisbeeFieldController* controller);
		void SetPursuitAgent(class FedoraAgent* agent);

		class FrisbeeFieldController* GetFieldController() const;

		// Fedora Agent Finders/Specifiers for Signals
		class FedoraAgent* GetPursuitAgent() const;
		class FedoraAgent* GetAgentWithFedora() const;

		class FedoraAgent* FindClosestAgent(Vector3 position) const;
		class FedoraAgent* FindClosesetAgentToFedora() const;
	private:
		StateMachine<Team> m_StateMachine;
		std::vector<class FedoraAgent*> m_Agents;
		TeamColor m_Color;
		TeamPlay m_Play;
		class FedoraAgent* m_ControllingAgent;
		class FedoraAgent* m_PursuitAgent;
		class FrisbeeFieldController* m_FieldController;
	};
}