#include <FedPCH.h>
#include <Objects/Fedora/Fedora.h>
#include <Objects/Agents/FedoraAgent.h>
#include "FrisbeeFieldController.h"

namespace Fed
{
	FrisbeeFieldController::FrisbeeFieldController() : m_Fedora(nullptr), m_LastThrownAgentID(0)
	{
		FrisbeeThrown.AddObserver(this);
	}
	// Returns whether given agent is touching fedora
	bool FrisbeeFieldController::IsAgentCollidingFedora(FedoraAgent * agent)
	{
		ASSERT(agent != nullptr && m_Fedora != nullptr, "Agent Colliding Fedora- NullPtr Exception");
		return agent->IsColliding(*m_Fedora);
	}
	// Returns first agent pointer that given agent is colliding with
	const FedoraAgent* FrisbeeFieldController::FindAgentCollidingAgent(const FedoraAgent * agent)
	{
		ASSERT(agent != nullptr, "Agent Colliding Agent- NullPtr Exception");

		for (const FedoraAgent* other : m_Agents)
		{
			if (agent != other && agent->IsColliding(*other))
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
	// Can agent pickup fedora. Fedora has no owner
	bool FrisbeeFieldController::IsFedoraFree() const
	{
		ASSERT(m_Fedora != nullptr, "FedoraPtr is null");
		return m_Fedora->GetOwner() == nullptr;
	}
	float FrisbeeFieldController::GetFedoraLaunchSpeed() const
	{
		ASSERT(m_Fedora != nullptr, "Must have fedora reference");
		return m_Fedora->GetLaunchSpeed();
	}
	// Queries to check if certain agent has fedora
	bool FrisbeeFieldController::AgentHasFedora(const FedoraAgent * agent)
	{
		ASSERT(m_Fedora != nullptr, "Must have fedora reference");
		ASSERT(agent != nullptr, "Must have agent reference");
		return (m_Fedora->GetOwner() && m_Fedora->GetOwner()->GetID() == agent->GetID());
	}
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
	// Returns whether agent is currently in fedora's path
	bool FrisbeeFieldController::IsAgentInFedoraPath(const FedoraAgent * agent)
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
	// Returns whether an agent can reach a moving fedora. Also returns intercept position if given a pointer
	bool FrisbeeFieldController::CanAgentInterceptFedora(const FedoraAgent * agent, Vector3 * outInterceptPos)
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
				//LOG("Can intercept. Fedora Speed: {0}", m_Fedora->GetSpeed());
				if (outInterceptPos != nullptr)
					*outInterceptPos = interceptPoint;
				return true;
			}
			//LOG("Cannot intercept. Time Ahead: {0}   -   Time To Intercept: {1}", timeAhead, timeToIntercept);
			//LOG("Cannot intercept. Intecept Point: ({0}, {1}, {2})", interceptPoint.x, interceptPoint.y, interceptPoint.z);
			//LOG("Cannot intercept. Dist to Intercept: {0}   --   Agent Max Speed: {1}", distToIntercept, agent->GetMaxSpeed());
		}
		return false;
	}
	Vector3 FrisbeeFieldController::GetFedoraPosition() const
	{
		ASSERT(m_Fedora != nullptr, "Fedora Position cannot be retrieved");
		return m_Fedora->ObjectTransform.Position;
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
	const int FrisbeeFieldController::GetLastThrownAgentID() const
	{
		return m_LastThrownAgentID;
	}
	Team * FrisbeeFieldController::GetTeam(TeamColor color) const
	{
		return (TeamColor::Blue ? &m_BlueTeam : &m_RedTeam);
	}
	void FrisbeeFieldController::OnEvent(const Subject * subject, Event & event)
	{
		Evnt::Dispatch<FrisbeeThrownEvent>(event, EVENT_BIND_FN(FrisbeeFieldController, OnFedoraThrown));
	}
	bool FrisbeeFieldController::OnFedoraThrown(FrisbeeThrownEvent & e)
	{
		m_LastThrownAgentID = e.GetAgent().GetID();
		return false;
	}
}