#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FedGame/FrisbeeFieldController/FrisbeeFieldController.h>
#include <EventSystem/Events/TeamSignal.h>
#include "TeamStates/TeamStates.h"
#include "Team.h"

namespace Fed
{
	Team::Team() : m_StateMachine(this, TeamStates::Standoff::Instance()), m_PositionFinder(this)
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
	// Specialized function to stun an agent from within a collision
	void Team::StunAgent(int agentID, float time)
	{
		for (auto it : m_Agents)
		{
			if (it->GetID() == agentID) {
				StunSignal stun(time);
				it->OnEvent(nullptr, stun);
				return;
			}
		}
		ASSERT(false, "Can't stun this agent as it doesn't exist");
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
		m_PositionFinder.Update();
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
			WaitSignal waitSignal;
			BroadcastSignal(waitSignal);
			ScoreSignal signal;
			agentWithFedora->OnEvent(nullptr, signal);
		} else if (play == TeamPlay::Defensive) {
			m_StateMachine.ChangeState(TeamStates::Defend::Instance());

			DefendSignal signal;
			BroadcastSignal(signal);

			FedoraAgent* assignedStealer = FindClosesetAgentToFedora();
			StealSignal stealSignal(*agentWithFedora);
			assignedStealer->OnEvent(nullptr, stealSignal);
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
	// Calculates risk rating by summing inverse of distances to each agent. Used by enemy team
	float Team::CalculateRiskAtPos(Vector3 position)
	{
		float risk = 0.0f;
		for (FedoraAgent* agent : m_Agents) {
			float dist = glm::length2(position - agent->ObjectTransform.Position);
			ASSERT(dist != 0, "Cannot divide by zero in safety calculation");
			risk += 1 / dist;
		}
		return risk;
	}
	// Draws the red blocks on the field to indicate the score in desirability for that position for the AI
	void Team::DebugRenderPositionScores() const
	{
		m_PositionFinder.DebugRender();
	}
	std::vector<Vector3> Team::GetAgentPositions() const
	{
		std::vector<Vector3> positions;
		for (const FedoraAgent* agent : m_Agents) {
			positions.push_back(agent->ObjectTransform.Position);
		}
		return positions;
	}
	// Loops agents to see if they can intercept fedora throw
	// Will also return the first agent it finds that can intercept. May change later to find best agent instead of first.
	bool Team::CanInterceptFedoraThrow(Vector3 throwPos, Vector3 targetPos, FedoraAgent* outInterceptAgent, Vector3* outInterceptPos) const
	{
		Vector3 toTarget = targetPos - throwPos;
		for (FedoraAgent* agent : m_Agents) {
			Vector3 toAgent = agent->ObjectTransform.Position - throwPos;
			// Ignore if agent is behind throw
			if (glm::dot(toAgent, toTarget) <= 0) {
				continue;
			}
			if (m_FieldController->CanAgentInterceptFedora(agent, outInterceptPos)) {
				outInterceptAgent = agent;
				return true;
			}

		}
		return false;
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
		float passingRisk = passing->CalculateRisk(); // how dangerous of a situation is passing agent in
		for (FedoraAgent* potential : m_Agents) {
			// Can't be itself
			if (potential == passing)
				continue;

			float potentialRisk = potential->CalculateRisk();
			// Must have lower risk
			if (potentialRisk > passingRisk)
				continue;

			// TODO: Replace this with fancier algorithm
			outPassPosition = potential->GetAgentPredictedPosition(passing->ObjectTransform.Position, passing->GetFieldController()->GetFedoraLaunchSpeed());  
			
			return potential;
		}
		return nullptr;
	}
}