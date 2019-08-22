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
	// Move to goal to score!
	void MoveToScore::Enter(FedoraAgentInputAI* owner)
	{
	}
	void MoveToScore::Execute(FedoraAgentInputAI* owner)
	{
		FrisbeeFieldController* controller = owner->GetOwner()->GetFieldController();
		const std::vector<GoalTrigger>& goals = controller->GetCourt()->GetGoals();
		TeamColor color = owner->GetOwner()->GetTeam()->GetColor();
		const GoalTrigger& targetGoal = (color != goals[0].Color ? goals[0] : goals[1]);

		Vector3 targetPos = targetGoal.Object.ObjectTransform.GetGlobalPosition();

		// If Path Blocked, Throw To Nearest Teammate

		// Actually Move to Goal
		float facingSpeed = 6.5f;
		owner->FaceTowards(targetPos, facingSpeed);
		owner->MoveTowards(targetPos);

	}
	void MoveToScore::Exit(FedoraAgentInputAI* owner)
	{
	}
};