#pragma once
#include <vector>
#include <StateMachine/StateMachine.h>
#include "AIPositionFinder/AIPositionFinder.h"

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
		void StunAgent(int agentID, float time = 1.0f);
		bool HasAgent(int agentID) const;
		void AddAgent(class FedoraAgent* agent);
		void Update();
		void RemoveAgent(class FedoraAgent* agent);
		void ResetPositions();
		void ExitStandoff();
		void SetTeamPlay(TeamPlay play, class FedoraAgent* agentWithFedora);
		void SetFieldControllerReference(class FrisbeeFieldController* controller);
		void SetPursuitAgent(class FedoraAgent* agent);
		float CalculateRiskAtPos(Vector3 position);
		void DebugRenderPositionScores() const;
		std::vector<Vector3> GetAgentPositions() const;
		bool CanInterceptFedoraThrow(Vector3 throwPos, Vector3 targetPos, 
			FedoraAgent* outInterceptAgent = nullptr, Vector3* outInterceptPos = nullptr) const;

		class FrisbeeFieldController* GetFieldController() const;

		// Fedora Agent Finders/Specifiers for Signals
		class FedoraAgent* GetPursuitAgent() const;
		class FedoraAgent* GetAgentWithFedora() const;

		class FedoraAgent* FindClosestAgent(Vector3 position) const;
		class FedoraAgent* FindClosesetAgentToFedora() const;
		class FedoraAgent* FindPassToAgent(class FedoraAgent* passing, Vector3& outPassPosition) const;
	private:
		AIPositionFinder m_PositionFinder;
		StateMachine<Team> m_StateMachine;
		std::vector<class FedoraAgent*> m_Agents;
		TeamColor m_Color;
		TeamPlay m_Play;
		class FedoraAgent* m_ControllingAgent;
		class FedoraAgent* m_PursuitAgent;
		class FrisbeeFieldController* m_FieldController;
	};
}