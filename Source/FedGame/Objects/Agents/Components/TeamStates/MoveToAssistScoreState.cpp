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
	void MoveToAssistScore::Enter(FedoraAgentInputAI* owner)
	{
	}
	void MoveToAssistScore::Execute(FedoraAgentInputAI* owner)
	{
		FedoraAgent* agent = owner->GetOwner();
		FrisbeeFieldController* controller = agent->GetFieldController();

		Vector3 targetPos = controller->GetTeam(agent->GetTeam()->GetColor())->GetBestAssistPosition();

		// Actually Move to Assist Position
		float facingSpeed = 3.5f;
		owner->FaceTowards(targetPos, facingSpeed);
		if (owner->MoveTowards(targetPos))
		{
			owner->FaceTowards(controller->GetFedoraPosition(), facingSpeed);
		}
		// TODO: once reaching target position, wait for pass

	}
	void MoveToAssistScore::Exit(FedoraAgentInputAI* owner)
	{
	}
};