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
	// Can agent pickup fedora. Fedora has no owner
	bool FrisbeeFieldController::IsFedoraFree() const
	{
		ASSERT(m_Fedora != nullptr, "FedoraPtr is null");
		return m_Fedora->GetOwner() == nullptr;
	}
	// Sets pointer to fedora.
	void FrisbeeFieldController::SetFedoraReference(Fedora * fedora)
	{
		if (fedora == m_Fedora)
			return;
		if (m_Fedora != nullptr)
			FrisbeeThrown.RemoveObserver(m_Fedora);
		if (fedora != nullptr)
			FrisbeeThrown.AddObserver(fedora);
		m_Fedora = fedora;
	}
}