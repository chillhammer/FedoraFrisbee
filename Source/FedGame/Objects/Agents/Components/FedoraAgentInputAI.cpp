#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
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
	// Returns whether one can intercept fedora.
	// Also updates intercept position variable
	bool FedoraAgentInputAI::CanInterceptFedora()
	{
		return GetOwner()->GetFieldController()->CanAgentInterceptFedora(GetOwner(), &m_InterceptPosition);
	}
	// Get Intercept Position for Agent to Travel to. Flatten the Y
	Vector3 FedoraAgentInputAI::GetInterceptPosition() const
	{
		Vector3 pos = m_InterceptPosition;
		pos.y = 0;
		return pos;
	}
	// Provides access to state machine. Used from within state machine
	StateMachine<FedoraAgentInputAI>& FedoraAgentInputAI::GetFSM()
	{
		return m_StateMachine;
	}
}