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
	// Chasing Frisbee in a smart way
	void Intercept::Enter(FedoraAgentInputAI* owner)
	{
	}
	void Intercept::Execute(FedoraAgentInputAI* owner)
	{
		FedoraAgent* agent = owner->GetOwner();
		float facingSpeed = 6.5f;
		owner->FaceTowards(owner->GetInterceptPosition(), facingSpeed);
		if (owner->MoveTowards(owner->GetInterceptPosition()))
		{
			owner->GetFSM().ChangeState(Wait::Instance());
		}
		if (!agent->GetFieldController()->IsFedoraMoving())
		{
			owner->GetFSM().ChangeState(ChaseFrisbee::Instance());
		}

	}
	void Intercept::Exit(FedoraAgentInputAI* owner)
	{
	}
}