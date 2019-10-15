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
		Team(TeamColor);
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
		bool InStandoff() const;
		void SetTeamPlay(TeamPlay play, class FedoraAgent* agentWithFedora);
		void SetFieldControllerReference(class FrisbeeFieldController* controller);
		void SetPursuitAgent(class FedoraAgent* agent);
		float CalculateRiskAtPos(Vector3 position);
		float CalculateDistSqrToClosestAgent(Vector3 position) const;
		void DebugRenderPositionScores() const;
		std::vector<Vector3> GetAgentPositions() const;
		Vector3 GetBestAssistPosition() const;
		bool CanInterceptFedoraThrow(Vector3 throwPos, Vector3 targetPos, 
			FedoraAgent** outInterceptAgent = nullptr, Vector3* outInterceptPos = nullptr) const;

		class FrisbeeFieldController* GetFieldController() const;

		// Fedora Agent Finders/Specifiers for Signals
		class FedoraAgent* GetPursuitAgent() const;
		class FedoraAgent* GetAgentWithFedora() const;

		class FedoraAgent* FindClosestAgent(Vector3 position) const;
		class FedoraAgent* FindClosesetAgentToFedora() const;
		class FedoraAgent* FindClosestAgentToEnemyGoal(int rank = 0, class FedoraAgent* agentToIgnore = nullptr);
		class FedoraAgent* FindClosestAgentToRay(Vector3 origin, Vector3 dir, class FedoraAgent* agentToIgnore = nullptr);
		class FedoraAgent* FindPassToAgent(class FedoraAgent* passing, Vector3& outPassPosition) const;
		class FedoraAgent* GetAgent(int index) const;

		int Score;
	private:
		TeamColor m_Color;
		AIPositionFinder m_PositionFinder;
		StateMachine<Team> m_StateMachine;
		std::vector<class FedoraAgent*> m_Agents;
		TeamPlay m_Play;
		class FedoraAgent* m_ControllingAgent;
		class FedoraAgent* m_PursuitAgent;
		class FrisbeeFieldController* m_FieldController;
	};
}