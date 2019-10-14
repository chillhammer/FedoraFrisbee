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
	Team::Team(TeamColor color) : m_StateMachine(this, TeamStates::Standoff::Instance()), m_Color(color), m_PositionFinder(this)
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

			AssistScoreSignal assistSignal;
			FindClosestAgentToEnemyGoal(0, agentWithFedora)->OnEvent(nullptr, assistSignal);

		} else if (play == TeamPlay::Defensive) {
			m_StateMachine.ChangeState(TeamStates::Defend::Instance());

			//DefendSignal signal;
			//BroadcastSignal(signal);

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
	// Returns distance to closest agent
	float Team::CalculateDistSqrToClosestAgent(Vector3 position) const
	{
		float smallestDist = -1;
		for (FedoraAgent* agent : m_Agents) {
			float dist = glm::length2(position - agent->ObjectTransform.Position);
			if (dist < smallestDist || smallestDist == -1) {
				smallestDist = dist;
			}
		}
		return smallestDist;
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
	Vector3 Team::GetBestAssistPosition() const
	{
		return m_PositionFinder.GetBestPosition();
	}
	// Loops agents to see if they can intercept fedora throw
	// Will also return the first agent it finds that can intercept. May change later to find best agent instead of first.
	bool Team::CanInterceptFedoraThrow(Vector3 throwPos, Vector3 targetPos, FedoraAgent** outInterceptAgent, Vector3* outInterceptPos) const
	{
		Vector3 toTarget = targetPos - throwPos;
		for (FedoraAgent* agent : m_Agents) {
			Vector3 toAgent = agent->ObjectTransform.Position - throwPos;
			// Ignore if agent is behind throw
			if (glm::dot(toAgent, toTarget) <= 0) {
				continue;
			}
			float distToFedora = glm::length(toAgent);
			float distToTarget = glm::length(toTarget);

			// Ignore if agent is farther than target
			if (distToTarget < distToFedora)
				continue;

			float fedoraSpeed = m_FieldController->GetFedoraLaunchSpeed();
			float timeAhead = distToFedora / fedoraSpeed; // estimation, not counting deceleration

			float distFedoraTraveled = glm::min(timeAhead * fedoraSpeed, m_FieldController->GetFedoraRange());
			Vector3 interceptPoint = throwPos + glm::normalize(toTarget) * distFedoraTraveled;

			float distToIntercept = glm::length(agent->ObjectTransform.Position - interceptPoint);
			float timeToIntercept = distToIntercept / agent->GetMaxSpeed();

			float distToInterceptFromTarget = glm::length(targetPos - interceptPoint);

			// If there is enough time to get into position
			// or can reach interception point before agent at target location
			if (timeToIntercept < timeAhead || distToIntercept < distToInterceptFromTarget) {
				if (outInterceptAgent)
					*outInterceptAgent = agent;
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
		for (FedoraAgent* agent : m_Agents) {
			if (m_FieldController->AgentHasFedora(agent))
				return agent;
		}
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
	FedoraAgent* Team::FindClosestAgentToEnemyGoal(int rank, FedoraAgent* agentToIgnore)
	{
		ASSERT(rank < m_Agents.size(), "Trying to find rank beyond agent list");
		float zEnemyGoal = m_FieldController->GetCourt()->GetGoalPosition(m_FieldController->GetEnemyTeam(this)).z;

		auto sortedAgents = m_Agents;
		std::sort(sortedAgents.begin(), sortedAgents.end(),
		[zEnemyGoal](const FedoraAgent* a, const FedoraAgent* b)
		{
			float aScore = glm::abs(a->ObjectTransform.Position.z - zEnemyGoal);
			float bScore = glm::abs(b->ObjectTransform.Position.z - zEnemyGoal);
			return aScore < bScore;
		});

		FedoraAgent* resultingAgent = sortedAgents[rank];

		if (resultingAgent == agentToIgnore) {
			// If last agent, then return none
			if (rank == m_Agents.size() - 1) {
				LOG_WARN("Cannot find closest agent to goal due to AgentToIgnore in Team::FindClosestAgentToEnemyGoal()");
				return nullptr;
			}
			else {
				resultingAgent = sortedAgents[rank + 1];
			}
		}
		return resultingAgent;
	}
	// find closest agent to ray
	FedoraAgent* Team::FindClosestAgentToRay(Vector3 origin, Vector3 dir, FedoraAgent* agentToIgnore)
	{
		FedoraAgent* closestAgent = nullptr;
		float smallestDistSqr = -1;
		for (FedoraAgent* agent : m_Agents) {
			if (agent == agentToIgnore) {
				continue;
			}
			Vector3 toAgent = agent->ObjectTransform.Position - origin;
			float dot = glm::dot(toAgent, dir);
			if (dot <= 0)
				continue;

			Vector3 projected = dot * dir + origin;
			float distSqr = glm::length2(projected - agent->ObjectTransform.Position);
			if (closestAgent == nullptr || distSqr < smallestDistSqr) {
				closestAgent = agent;
				smallestDistSqr = distSqr;
			}
		}
		return closestAgent;
	}
	// Find agent to pass to, if any
	FedoraAgent* Team::FindPassToAgent(FedoraAgent* passing, Vector3& outPassPosition) const
	{
		ASSERT(passing->GetHasFedora(), "Agent must have fedora to find pass!");
		float passingRisk = passing->CalculateRisk(); // how dangerous of a situation is passing agent in
		FrisbeeFieldController* fieldController = GetFieldController();
		Team* enemyTeam = fieldController->GetEnemyTeam(GetColor());
		float fedoraSpeed = fieldController->GetFedoraLaunchSpeed();

		FedoraAgent* bestAgent = nullptr;
		Vector3 bestPassingLoc = Vector3(0.0f, 0.0f, 0.0f);
		float lowestRisk = -1;
		for (FedoraAgent* potential : m_Agents) {
			// Can't be itself
			if (potential == passing)
				continue;

			// Find best passing location
			Vector3 agentPos = potential->ObjectTransform.Position;
			float distToAgent = glm::length(agentPos - passing->ObjectTransform.Position);
			Vector3 dirToAgent = glm::normalize(agentPos - passing->ObjectTransform.Position);
			Vector3 agentSide = Vector3(-dirToAgent.z, 0.0f, dirToAgent.x);
			ASSERT(glm::abs(glm::length2(agentSide) - 1.0f) < 0.1f, "Side dir is not normalized");

			// Ignore close agents
			float closeThreshold = 3.0f;
			if (distToAgent < closeThreshold) {
				continue;
			}

			// Ignore agents with enemy on them
			if (enemyTeam->CalculateDistSqrToClosestAgent(agentPos) < enemyTeam->CalculateDistSqrToClosestAgent(passing->ObjectTransform.Position)) {
				continue;
			}
			
			float fedoraTravelDist = glm::min(distToAgent, fieldController->GetFedoraRange());
			float sideDist = potential->GetMaxSpeed() * (fedoraTravelDist / fedoraSpeed) * 0.9f;

			std::vector<Vector3> potentialLocs{ agentPos, agentPos + agentSide * sideDist, agentPos - agentSide * sideDist };

			float agentLowestRisk = -1;
			Vector3 agentBestLoc = Vector3(0.0f, 0.0f, 0.0f);

			// Finding best passing location for this agent
			if (!potential->IsPlayerControlled()) {
				for (Vector3 loc : potentialLocs) {
					// Make sure within field
					if (!fieldController->GetCourt()->IsPointWithinField(loc))
						continue;

					// Make sure passable
					if (enemyTeam->CanInterceptFedoraThrow(passing->ObjectTransform.Position, loc)) {
						//LOG("Cannot pass to location: x={0}, y={1}", loc.x, loc.z);
						continue;
					}

					// Make sure no agent in path
					//const FedoraAgent* blockingAgent = fieldController->FindAgentInAgentPath(passing, loc - dirToAgent * 2.0f);
					//if (blockingAgent != nullptr)
					//	continue;

					// Find lowest risk
					//float risk = enemyTeam->CalculateRiskAtPos(loc);
					float risk = glm::length2(fieldController->GetCourt()->GetGoalPosition(enemyTeam) - loc); // try distance to goal as risk
					if (agentLowestRisk < 0 || agentLowestRisk > risk) {
						agentLowestRisk = risk;
							agentBestLoc = loc;
					}
				}
			}
			else {
				// If passing to human, always pass based on their velocity.
				agentBestLoc = potential->GetAgentPredictedPosition(passing->ObjectTransform.Position, fedoraSpeed);

				// Make sure within field
				if (!fieldController->GetCourt()->IsPointWithinField(agentBestLoc))
					continue;

				// Make sure passable
				if (enemyTeam->CanInterceptFedoraThrow(passing->ObjectTransform.Position, agentBestLoc)) {
					continue;
				}
				//agentLowestRisk = enemyTeam->CalculateRiskAtPos(agentBestLoc);
				agentLowestRisk = glm::length2(fieldController->GetCourt()->GetGoalPosition(enemyTeam) - agentBestLoc); // try distance to goal as risk
			}

			// Ignore agents in riskier positions
			//if (agentLowestRisk > passingRisk)
			//	continue;

			// Global search
			if (lowestRisk == -1 || agentLowestRisk < lowestRisk) {
				lowestRisk = agentLowestRisk;
				
				if (lowestRisk != -1) { // only change if found suitable target
					bestAgent = potential;
					bestPassingLoc = agentBestLoc;
				}
			}

		}
		outPassPosition = bestPassingLoc;
		return bestAgent;
	}
	FedoraAgent* Team::GetAgent(int index) const
	{
		return m_Agents[index];
	}
}