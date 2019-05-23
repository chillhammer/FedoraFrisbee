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
	// Doing nothing
	void Wait::Enter(FedoraAgentInputAI* owner)
	{
	}
	void Wait::Execute(FedoraAgentInputAI* owner)
	{
		FedoraAgent* agent = owner->GetOwner();
		FrisbeeFieldController* fieldController = agent->GetFieldController();

		// Face Frisbee
		Vector3 pointToFace = fieldController->GetFedoraPosition();
		float facingSpeed = 55.5f;
		if (!agent->GetHasFedora())
			owner->FaceTowards(pointToFace, facingSpeed);

	}
	void Wait::Exit(FedoraAgentInputAI* owner)
	{
	}
}