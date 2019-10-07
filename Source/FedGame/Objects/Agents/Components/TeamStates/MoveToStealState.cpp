#include <FedPCH.h>
#include <Game/GameManager.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include "FedoraAgentInputAITeamStates.h"
/**
	Each state runs code that determines Fedora, the hat, behavior
**/
namespace Fed::AgentAITeamStates
{
	// Move to target agent to retrieve fedora
	void MoveToSteal::Enter(FedoraAgentInputAI* owner)
	{
	}
	void MoveToSteal::Execute(FedoraAgentInputAI* owner)
	{
		// If just stolen from, delay before chasing captor
		if (owner->IsStunned()) {
			return;
		}
		FedoraAgent* agent = owner->GetOwner();
		FrisbeeFieldController* controller = agent->GetFieldController();
		
		ASSERT(owner->GetTargetAgent(), "Must have target agent to steal from");
		if (!owner->GetTargetAgent()->GetHasFedora()) {
			owner->GetFSM().ChangeState(AgentAITeamStates::Pursue::Instance());
			return;
		}
		float facingSpeed = 6.5f;
		owner->MoveToSteal();
		owner->FaceTowards(owner->GetTargetAgent()->ObjectTransform.Position, facingSpeed);
	}
	void MoveToSteal::Exit(FedoraAgentInputAI* owner)
	{
	}
};