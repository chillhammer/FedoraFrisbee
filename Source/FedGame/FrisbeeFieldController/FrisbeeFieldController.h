#pragma once
#include <EventSystem/Events/FrisbeeFieldEvent.h>
#include <EventSystem/Subject.h>
#include <Objects/Agents/FedoraAgent.h>
#include <Objects/Fedora/Fedora.h>
#include <Objects/Court/Court.h>
#include "Team/Team.h"
#include <vector>
namespace Fed
{

	/**
		Controls the field and logic of a Fedora Frisbee match.
		Handles events between players, and also with frisbee
		An API for agents to query information about the field, agents, fedora
	**/
	class FrisbeeFieldController : IObserver
	{
	public:
		FrisbeeFieldController();

		bool IsAgentCollidingFedora(FedoraAgent* agent);
		const FedoraAgent* FindAgentCollidingAgent(const FedoraAgent* agent);
		const FedoraAgent* FindPlayerAgent();
		const FedoraAgent* GetAgentFromID(int id) const;
		const FedoraAgent* FindAgentInAgentPath(const FedoraAgent* agent, Vector3 target) const;
		bool AgentHasFedora(const FedoraAgent* agent) const;
		void AddAgentReference(const FedoraAgent* agent);
		bool IsAgentInFedoraPath(const FedoraAgent* agent) const;
		bool CanAgentInterceptFedora(const FedoraAgent* agent, Vector3* __restrict outInterceptPos);
		bool CanEnemyInterceptFedoraThrow(const Team* throwTeam, Vector3 throwPos, Vector3 targetPos, 
			FedoraAgent** outInterceptAgent = nullptr, Vector3* outInterceptPos = nullptr);
		bool IsFedoraFree() const;
		bool IsFedoraMoving() const;
		bool GetScored() const;
		void StunAgent(const FedoraAgent* agent, float time = 1.0f);
		float		GetFedoraLaunchSpeed() const;
		float		GetFedoraRange() const;
		Vector3		GetFedoraPosition() const;
		const int	GetLastThrownAgentID() const;
		Team*		GetTeam(TeamColor color);
		Team*		GetEnemyTeam(TeamColor color);
		Team*		GetEnemyTeam(Team* team);
		Court*		GetCourt() const;
		bool		GetGameEnded() const;
		void		SetGameEnded(bool gameEnded);
		void		SetFedoraReference(Fedora* fedora);
		void		SetCourtReference(class Court* court);
		void		ResetPositions();
		void OnEvent(const Subject* subject, Event& event);
	public:
		Subject FrisbeeThrown;
		Subject FrisbeePickup;
		Subject FrisbeeScored;
		Subject GameEnded;

	private:
		bool OnFedoraThrown(FrisbeeThrownEvent& e);
		bool OnFedoraScored(FrisbeeScoredEvent& e);
		bool OnFedoraPickup(FrisbeePickupEvent& e);
	private:
		Fedora* m_Fedora;
		Court* m_Court;
		std::vector<const FedoraAgent*> m_Agents;
		std::unordered_map<int, const FedoraAgent*> m_AgentLookup;
		int m_LastThrownAgentID;
		Team m_BlueTeam;
		Team m_RedTeam;
		bool m_Scored;
		int m_ScoreLimit;
		bool m_GameEnded;
	};
}