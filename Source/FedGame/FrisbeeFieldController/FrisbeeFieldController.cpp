#include <FedPCH.h>
#include <Objects/Fedora/Fedora.h>
#include <Objects/Agents/FedoraAgent.h>
#include "FrisbeeFieldController.h"

namespace Fed
{
	FrisbeeFieldController::FrisbeeFieldController() : m_Fedora(nullptr)
	{
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
	// Can agent pickup fedora. Fedora has no owner
	bool FrisbeeFieldController::IsFedoraFree() const
	{
		ASSERT(m_Fedora != nullptr, "FedoraPtr is null");
		return m_Fedora->GetOwner() == nullptr;
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
		}
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
}