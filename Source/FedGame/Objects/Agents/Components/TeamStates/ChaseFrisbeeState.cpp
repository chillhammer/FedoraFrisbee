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
	// Simple Chase of Frisbee
	void ChaseFrisbee::Enter(FedoraAgentInputAI* owner)
	{
	}
	void ChaseFrisbee::Execute(FedoraAgentInputAI* owner)
	{
		FrisbeeFieldController* controller = owner->GetOwner()->GetFieldController();
		Vector3 frisbeePos = controller->GetFedoraPosition();
		frisbeePos.y = 0;
		float facingSpeed = 6.5f;
		owner->FaceTowards(frisbeePos, facingSpeed);
		owner->MoveTowards(frisbeePos);

		// Stop chasing if it is picked up
		if (!controller->IsFedoraFree())
		{
			owner->GetFSM().ChangeState(Wait::Instance());
		}

	}
	void ChaseFrisbee::Exit(FedoraAgentInputAI* owner)
	{
	}
}