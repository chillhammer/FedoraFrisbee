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
		FedoraAgent* agent = owner->GetOwner();
		FrisbeeFieldController* controller = agent->GetFieldController();
		const std::vector<GoalTrigger>& goals = controller->GetCourt()->GetGoals();
		TeamColor color = agent->GetTeam()->GetColor();
		const GoalTrigger& targetGoal = (color != goals[0].Color ? goals[0] : goals[1]);

		ASSERT(targetGoal.Object.ObjectTransform.Position == Vector3(0, 0, 0), "Assumption that goals' position has no offset is false.");
		Vector3 targetPos = targetGoal.Object.GetBoundingBox().GetCenter(); targetPos.y = 0;

		// If Path Blocked, Throw To Nearest Teammate
		const FedoraAgent* blocking = controller->FindAgentInAgentPath(agent, targetPos);
		if (blocking != nullptr) {
			// TODO: find teammate closest to enemy goal
			const Team* team = agent->GetTeam();
			Vector3 throwToPos;
			// TODO: fix find pass function
			const FedoraAgent* passToAgent = team->FindPassToAgent(agent, throwToPos);

			if (passToAgent) {
				if (owner->FaceTowards(throwToPos, 10.f)) {
					owner->ThrowFrisbee(agent);
					owner->GetFSM().ChangeState(AgentAITeamStates::Wait::Instance());
				}
				return;
			}
		}

		// Actually Move to Goal
		float facingSpeed = 6.5f;
		owner->FaceTowards(targetPos, facingSpeed);
		owner->SeekAndAvoidEnemies(targetPos);

	}
	void MoveToScore::Exit(FedoraAgentInputAI* owner)
	{
	}
};