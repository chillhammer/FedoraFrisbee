#include <FedPCH.h>
#include "FedoraAgentInputAIStates.h"
#include "FedoraAgentInputAI.h"

namespace Fed
{
	Fed::FedoraAgentInputAI::FedoraAgentInputAI() : m_StateMachine(this, AgentAIStates::Wait::Instance())
	{
	}
	// Handles Events
	void FedoraAgentInputAI::OnEvent(const Subject * subject, Event & e)
	{
	}
	void FedoraAgentInputAI::Update(FedoraAgent * owner)
	{
		ASSERT(owner != nullptr, "Agent cannot be nullptr");
		// Stores owner for use within FSM
		m_Owner = owner;

		m_StateMachine.Update();
	}
	// Returns owner of this component. Used within FSM
	FedoraAgent * FedoraAgentInputAI::GetOwner() const
	{
		return m_Owner;
	}
}