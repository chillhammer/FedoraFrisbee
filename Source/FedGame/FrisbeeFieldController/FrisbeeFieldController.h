#pragma once
#include <EventSystem/Events/FrisbeeFieldEvent.h>
#include <EventSystem/Subject.h>
#include <Objects/Agents/FedoraAgent.h>
#include <Objects/Fedora/Fedora.h>
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
		bool AgentHasFedora(const FedoraAgent* agent);
		void AddAgentReference(const FedoraAgent* agent);
		bool IsAgentInFedoraPath(const FedoraAgent* agent);
		bool CanAgentInterceptFedora(const FedoraAgent* agent, Vector3* outInterceptPos);
		bool IsFedoraFree() const;
		float GetFedoraLaunchSpeed() const;
		Vector3 GetFedoraPosition() const;
		void SetFedoraReference(Fedora* fedora);
		const int GetLastThrownAgentID() const;
		void OnEvent(const Subject* subject, Event& event);
	public:
		Subject FrisbeeThrown;
		Subject FrisbeePickup;

	private:
		bool OnFedoraThrown(FrisbeeThrownEvent& e);
	private:
		class Fedora* m_Fedora;
		std::vector<const FedoraAgent*> m_Agents;
		int m_LastThrownAgentID;
	};
}