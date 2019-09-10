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
	// Move to best assist position
	void MoveToScore::Enter(FedoraAgentInputAI* owner)
	{
	}
	void MoveToScore::Execute(FedoraAgentInputAI* owner)
	{
		FedoraAgent* agent = owner->GetOwner();
		FrisbeeFieldController* controller = agent->GetFieldController();

		Vector3 targetPos = controller->GetTeam(agent->GetTeam()->GetColor())->GetBestAssistPosition();

		// Actually Move to Goal
		float facingSpeed = 3.5f;
		owner->FaceTowards(targetPos, facingSpeed);
		owner->MoveTowards(targetPos);

	}
	void MoveToAssistScore::Exit(FedoraAgentInputAI* owner)
	{
	}
};