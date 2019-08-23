#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FedGame/FrisbeeFieldController/FrisbeeFieldController.h>
#include <EventSystem/Events/TeamSignal.h>
#include "TeamStates/TeamStates.h"
#include "Team.h"

namespace Fed
{
	Team::Team() : m_StateMachine(this, TeamStates::Standoff::Instance())
	{
	}
	TeamPlay Team::GetPlay() const
	{
		return m_Play;
	}
	TeamColor Team::GetColor() const
	{
		return m_Color;
	}
	void Team::SetColor(TeamColor color)
	{
		m_Color = color;
	}
	void Team::BroadcastSignal(Event& event)
	{
		for (auto it : m_Agents)
		{
			it->OnEvent(nullptr, event);
		}
	}
	bool Team::HasAgent(int agentID) const
	{
		for (auto it : m_Agents)
		{
			if (it->GetID() == agentID)
				return true;
		}
		return false;
	}
	void Team::AddAgent(FedoraAgent * agent)
	{
		ASSERT(!HasAgent(agent->GetID()), "Can't add agent that exists");
		m_Agents.push_back(agent);
	}
	void Team::Update()
	{
		m_StateMachine.Update();
	}
	void Team::RemoveAgent(FedoraAgent * agent)
	{
		ASSERT(HasAgent(agent->GetID()), "Can't remove agent that doesn't exists");
		m_Agents.erase(std::remove(m_Agents.begin(), m_Agents.end(), agent), m_Agents.end());
	}
	void Team::ResetPositions()
	{
		m_StateMachine.ChangeState(TeamStates::Standoff::Instance());
		int side = (m_Color == TeamColor::Red ? -1 : 1);
		for (int i = 0; i < m_Agents.size(); i++) {
			FedoraAgent* agent = m_Agents[i];
			//agent->ObjectTransform.SetYaw(90 + side * 90);
			agent->SetFacingVector(Vector3(0, 0, -side));
			float horOffset = 5.0f;
			
			switch (i) {
			case 0:
				agent->ObjectTransform.Position = Vector3(0.0f, 0.0f, 20.0f * side);
				break;
			case 1:
				agent->ObjectTransform.Position = Vector3(horOffset, 0.0f, 15 * side);
				break;
			case 2:
				agent->ObjectTransform.Position = Vector3(-horOffset, 0.0f, 15 * side);
				break;

			}
		}
	}
	void Team::ExitStandoff()
	{
		m_StateMachine.ChangeState(TeamStates::Pursue::Instance());
	}
	// Sets team play and changes team state
	void Team::SetTeamPlay(TeamPlay play, FedoraAgent* agentWithFedora)
	{
		m_Play = play;
		if (play == TeamPlay::Offensive) {
			m_StateMachine.ChangeState(TeamStates::Score::Instance());
			ScoreSignal signal;
			agentWithFedora->OnEvent(nullptr, signal);
		} else if (play == TeamPlay::Defensive) {
			m_StateMachine.ChangeState(TeamStates::Defend::Instance());
			DefendSignal signal;
			agentWithFedora->OnEvent(nullptr, signal);
		}
	}
	void Team::SetFieldControllerReference(FrisbeeFieldController* controller)
	{
		m_FieldController = controller;
	}
	FrisbeeFieldController* Team::GetFieldController() const
	{
		return m_FieldController;
	}
	void Team::SetPursuitAgent(FedoraAgent* agent)
	{
		m_PursuitAgent = agent;
	}
	FedoraAgent* Team::GetPursuitAgent() const
	{
		return m_PursuitAgent;
	}
	// Returns agent with fedora. Asserts this team has the fedora
	FedoraAgent* Team::GetAgentWithFedora() const
	{
		ASSERT(false, "GetAgentWithFedora() incomplete");
		// TODO: Fill this out or delete function
		return nullptr;
	}
	// Calculates closest agent position-wise to position
	FedoraAgent* Team::FindClosestAgent(Vector3 position) const
	{
		FedoraAgent* closest = nullptr;
		float smallestDist = -1;
		float dist = -1;
		for (auto it : m_Agents)
		{
			dist = glm::length(it->ObjectTransform.Position - position);
			if (dist < smallestDist || smallestDist == -1) {
				smallestDist = dist;
				closest = it;
			}
		}
		return closest;
	}
	FedoraAgent* Team::FindClosesetAgentToFedora() const
	{
		return FindClosestAgent(m_FieldController->GetFedoraPosition());
	}
	// Find agent to pass to, if any
	FedoraAgent* Team::FindPassToAgent(FedoraAgent* passing, Vector3& outPassPosition) const
	{
		ASSERT(passing->GetHasFedora(), "Agent must have fedora to find pass!");
		for (FedoraAgent* potential : m_Agents) {
			if (potential != passing) {
				// TODO: Replace this with fancier algorithm
				outPassPosition = potential->ObjectTransform.GetGlobalPosition();
				return potential;
			}
		}
		return nullptr;
	}
}