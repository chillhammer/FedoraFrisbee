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
	// Intermediate state. Decides between intercept or chase
	void Pursue::Enter(FedoraAgentInputAI* owner)
	{
	}
	void Pursue::Execute(FedoraAgentInputAI* owner)
	{
		FedoraAgent* agent = owner->GetOwner();
		FrisbeeFieldController* fieldController = agent->GetFieldController();

		// Should only be pursuing if team does not have fedora

		// Try to intercept
		if (owner->CanInterceptFedora())
		{
			owner->GetFSM().ChangeState(Intercept::Instance());
		}
		// Otherwise chase
		else
		{
			owner->GetFSM().ChangeState(ChaseFrisbee::Instance());
		}

	}
	void Pursue::Exit(FedoraAgentInputAI* owner)
	{
	}
}