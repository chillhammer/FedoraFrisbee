#include <FedPCH.h>
#include <Objects/Fedora/Fedora.h>
#include <Objects/Agents/FedoraAgent.h>
#include <EventSystem/Events/TeamSignal.h>
#include "FrisbeeFieldController.h"

namespace Fed
{
	FrisbeeFieldController::FrisbeeFieldController() : m_Fedora(nullptr), m_LastThrownAgentID(0), m_BlueTeam(TeamColor::Blue), m_RedTeam(TeamColor::Red), m_Court(nullptr)
	{
		FrisbeeThrown.AddObserver(this);
		FrisbeeScored.AddObserver(this);
		FrisbeePickup.AddObserver(this);
		m_BlueTeam.SetFieldControllerReference(this);
		m_RedTeam.SetFieldControllerReference(this);

	}
	#pragma region Accessors
	// Returns whether given agent is touching fedora
	bool FrisbeeFieldController::IsAgentCollidingFedora(FedoraAgent * agent)
	{
		ASSERT(agent != nullptr && m_Fedora != nullptr, "Agent Colliding Fedora- NullPtr Exception");
		return agent->IsColliding(*m_Fedora);
	}
	// Loops over all agents naively
	// Returns first agent pointer that given agent is colliding with
	const FedoraAgent* FrisbeeFieldController::FindAgentCollidingAgent(const FedoraAgent * agent)
	{
		ASSERT(agent != nullptr, "Agent Colliding Agent- NullPtr Exception");

		for (const FedoraAgent* other : m_Agents)
		{
			if (agent != other && !other->IsInvincible() && agent->IsColliding(*other))
			{
				return other;
			}
		}
		return nullptr;
	}
	// Returns agent that is controlled by player. Nullptr if there is none
	const FedoraAgent * FrisbeeFieldController::FindPlayerAgent()
	{
		for (const FedoraAgent* agent : m_Agents)
		{
			if (agent->IsPlayerControlled())
				return agent;
		}
		return nullptr;
	}
	// Uses lookup table to find agent in O(1)
	const FedoraAgent * FrisbeeFieldController::GetAgentFromID(int id) const
	{
		auto it = m_AgentLookup.find(id);
		if (it != m_AgentLookup.end())
			return (*it).second;
		return nullptr;
	}
	// Returns first agent on path to target location
	const FedoraAgent* FrisbeeFieldController::FindAgentInAgentPath(const FedoraAgent* agent, Vector3 target) const
	{
		Vector3 loc = agent->ObjectTransform.GetGlobalPosition(); loc.y = 0;
		target.y = 0;
		float maxDist = glm::length(target - loc);
		float bestDist = -1;
		const FedoraAgent* closestAgent = nullptr;

		for (const FedoraAgent* a : m_Agents) {
			// Can't be itself
			if (a == agent)
				continue;
			Vector3 aPos = a->ObjectTransform.GetGlobalPosition();
			float dist = glm::length(aPos - loc);

			// Can't be farther than target
			if (dist > maxDist)
				continue;

			//Vector3 toA = glm::normalize(aPos - loc);
			Vector3 toT = glm::normalize(target - loc);

			Vector3 futurePos = loc + toT * dist;
			
			// If it collides, find closest agent
			if (agent->IsCollidingAtPosition(*a, futurePos)) {
				if (bestDist == -1 || dist < bestDist) {
					bestDist = dist;
					closestAgent = a;
				}
			}
		}

		return closestAgent;
	}
	// Can agent pickup fedora. Fedora has no owner
	bool FrisbeeFieldController::IsFedoraFree() const
	{
		ASSERT(m_Fedora != nullptr, "FedoraPtr is null");
		return m_Fedora->GetOwner() == nullptr;
	}
	bool FrisbeeFieldController::IsFedoraMoving() const
	{
		ASSERT(m_Fedora != nullptr, "Must have fedora reference");
		return m_Fedora->IsMoving();
	}
	bool FrisbeeFieldController::GetScored() const
	{
		return m_Scored;
	}
	void FrisbeeFieldController::StunAgent(const FedoraAgent* agent, float time)
	{
		agent->GetTeam()->StunAgent(agent->GetID(), time);
	}
	float FrisbeeFieldController::GetFedoraLaunchSpeed() const
	{
		ASSERT(m_Fedora != nullptr, "Must have fedora reference");
		return m_Fedora->GetLaunchSpeed();
	}
	float FrisbeeFieldController::GetFedoraRange() const
	{
		return m_Fedora->GetThrowRange();
	}
	// Queries to check if certain agent has fedora
	bool FrisbeeFieldController::AgentHasFedora(const FedoraAgent * agent) const
	{
		ASSERT(m_Fedora != nullptr, "Must have fedora reference");
		ASSERT(agent != nullptr, "Must have agent reference");
		return (m_Fedora->GetOwner() && m_Fedora->GetOwner()->GetID() == agent->GetID());
	}
	// Returns whether agent is currently in fedora's path
	bool FrisbeeFieldController::IsAgentInFedoraPath(const FedoraAgent * agent) const
	{
		ASSERT(agent != nullptr, "Agent must not be nullptr");
		if (IsFedoraFree() && m_Fedora->IsMoving())
		{
			Vector3 agentPos = agent->ObjectTransform.Position;
			Vector3 fedoraPos = GetFedoraPosition(); fedoraPos.y = 0;
			Vector3 toAgent = agentPos - fedoraPos;
			Vector3 fedoraHeading = m_Fedora->GetDirection();
			float toAgentDist = glm::length(toAgent);
			toAgent = glm::normalize(toAgent);
			float distFromLaunch = glm::length(agentPos - m_Fedora->GetLastThrownPosition());
			// Within reachable range
			if (distFromLaunch <= m_Fedora->GetThrowRange())
			{
				// Within colliding distance
				Vector3 closestPosOnFedoraTrajectory = fedoraPos + fedoraHeading * toAgentDist;
				float collidingDist = 0.7f;
				if (glm::length(closestPosOnFedoraTrajectory - agentPos) <= collidingDist)
				{
					return true;
				}
			}
		}
		return false;
	}
	// Returns whether an agent can reach a moving fedora. Also returns intercept position if given a pointer. Fedora must be in air
	bool FrisbeeFieldController::CanAgentInterceptFedora(const FedoraAgent * agent, Vector3 * __restrict outInterceptPos)
	{
		ASSERT(agent != nullptr && m_Fedora != nullptr, "Agent or fedora has nullptr");
		if (m_Fedora->IsMoving())
		{
			Vector3 dirToFedora = GetFedoraPosition() - agent->ObjectTransform.Position;
			if (glm::dot(dirToFedora, m_Fedora->GetDirection()) > 0.f)
			{
				return false;
			}
			float distToFedora = glm::length(dirToFedora);
			float timeAhead = distToFedora / m_Fedora->GetSpeed();

			Vector3 interceptPoint = m_Fedora->GetFuturePosition(timeAhead);
			float distToIntercept = glm::length(agent->ObjectTransform.Position - interceptPoint);
			float timeToIntercept = distToIntercept / agent->GetMaxSpeed();
			if (timeToIntercept <= timeAhead)
			{
				m_Fedora->GetFuturePosition(timeAhead, false); // Draw Debug - false
				if (outInterceptPos != nullptr)
					*outInterceptPos = interceptPoint;
				return true;
			}
		}
		return false;
	}
	// Args: throwTeam: team that will be throwing the fedora. 
	//		 throwPos:  position to launch fedora from. 
	//		 targetPos: position to throw fedora to
	// Returns whether given throw can be intercepted by any enemy
	bool FrisbeeFieldController::CanEnemyInterceptFedoraThrow(const Team* throwTeam, Vector3 throwPos, Vector3 targetPos, FedoraAgent** outInterceptAgent, Vector3* outInterceptPos)
	{
		Team* enemyTeam = GetEnemyTeam(throwTeam->GetColor());

		return enemyTeam->CanInterceptFedoraThrow(throwPos, targetPos, outInterceptAgent, outInterceptPos);
	}
	Vector3 FrisbeeFieldController::GetFedoraPosition() const
	{
		ASSERT(m_Fedora != nullptr, "Fedora Position cannot be retrieved");
		return m_Fedora->ObjectTransform.GetGlobalPosition();
	}
	const int FrisbeeFieldController::GetLastThrownAgentID() const
	{
		return m_LastThrownAgentID;
	}
	Team* FrisbeeFieldController::GetTeam(TeamColor color) 
	{
		return (color == TeamColor::Blue ? &m_BlueTeam : &m_RedTeam);
	}
	Team* FrisbeeFieldController::GetEnemyTeam(TeamColor color) 
	{
		return GetTeam((color == TeamColor::Blue ? TeamColor::Red : TeamColor::Blue));
	}
	Team* FrisbeeFieldController::GetEnemyTeam(Team* team) 
	{
		return GetEnemyTeam(team->GetColor());
	}
	Court* FrisbeeFieldController::GetCourt() const
	{
		return m_Court;
	}
	#pragma endregion

	#pragma region Setters / Modifiers
	// Adds agent to field controller reference
	// Allows it to keep track of all agents and make other functions work properly
	void FrisbeeFieldController::AddAgentReference(const FedoraAgent* agent)
	{
		if (std::find(m_Agents.begin(), m_Agents.end(), agent) == m_Agents.end())
		{
			m_Agents.emplace_back(agent);
			m_AgentLookup[agent->GetID()] = agent;
		}
	}

	// Sets pointer to fedora.
	void FrisbeeFieldController::SetFedoraReference(Fedora * fedora)
	{
		if (fedora == m_Fedora)
			return;
		if (m_Fedora != nullptr)
		{
			FrisbeeThrown.RemoveObserver(m_Fedora);
			FrisbeePickup.RemoveObserver(m_Fedora);
		}
		if (fedora != nullptr)
		{
			FrisbeeThrown.AddObserver(fedora);
			FrisbeePickup.AddObserver(fedora);
		}
		m_Fedora = fedora;
	}

	// Sets pointer to court
	void FrisbeeFieldController::SetCourtReference(Court * court)
	{
		m_Court = court;
	}
	void FrisbeeFieldController::ResetPositions()
	{
		m_Scored = false;
		m_BlueTeam.ResetPositions();
		m_RedTeam.ResetPositions();

		m_Fedora->ObjectTransform.Position = Vector3(0, 0, 0);
		m_Fedora->SetOwner(nullptr);
		m_Fedora->Stop();

		
	}
	#pragma endregion
	
	#pragma region Events
	void FrisbeeFieldController::OnEvent(const Subject * subject, Event & event)
	{
		Evnt::Dispatch<FrisbeeThrownEvent>(event, EVENT_BIND_FN(FrisbeeFieldController, OnFedoraThrown));
		Evnt::Dispatch<FrisbeeScoredEvent>(event, EVENT_BIND_FN(FrisbeeFieldController, OnFedoraScored));
		Evnt::Dispatch<FrisbeePickupEvent>(event, EVENT_BIND_FN(FrisbeeFieldController, OnFedoraPickup));
	}
	bool FrisbeeFieldController::OnFedoraThrown(FrisbeeThrownEvent & e)
	{
		// Update Last Thrown ID
		m_LastThrownAgentID = e.GetAgent().GetID();

		// Try to get enemy team to intercept or at least chase
		// Does not have information on intended pass target
		Team* enemyTeam = GetEnemyTeam(e.GetAgent().GetTeam());
		Vector3 throwPos = e.GetAgent().ObjectTransform.Position;
		FedoraAgent* enemyPursueAgent = nullptr;
		enemyTeam->CanInterceptFedoraThrow(throwPos, throwPos + m_Fedora->GetThrowRange() * e.GetDirection(), &enemyPursueAgent);

		if (enemyPursueAgent == nullptr) {
			// If no one can intercept, have closest person chase
			enemyPursueAgent = enemyTeam->FindClosesetAgentToFedora();
		}
		ASSERT(enemyPursueAgent, "Must have an enemy chasing the thrown fedora");
		PursueSignal signal;
		enemyPursueAgent->OnEvent(nullptr, signal);

		return false;
	}
	bool FrisbeeFieldController::OnFedoraScored(FrisbeeScoredEvent& e)
	{
		m_Fedora->SetCanScore(false);
		// TODO: Fancy effect when scoring
		if (!m_Scored) {
			GetEnemyTeam(e.GetScoringTeam())->Score++;
			m_Scored = true;

			WaitSignal signal;
			m_BlueTeam.BroadcastSignal(signal);
			m_RedTeam.BroadcastSignal(signal);
		}

		//ResetPositions();

		return false;
	}
	bool FrisbeeFieldController::OnFedoraPickup(FrisbeePickupEvent& e)
	{
		Team* pickup = e.GetAgent().GetTeam();
		Team* other = (&m_BlueTeam == pickup ? &m_RedTeam : &m_BlueTeam);
		pickup->SetTeamPlay(TeamPlay::Offensive, &e.GetAgent());
		other->SetTeamPlay(TeamPlay::Defensive, &e.GetAgent());
		LOG("Team {0} goes on Attack", pickup->GetColor() == TeamColor::Blue ? "Blue" : "Red");
		return false;
	}
	#pragma endregion
}