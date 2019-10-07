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
		// This blocked flag means if it's path is blocked and thus abort mission
		owner->SetBlocked(false);
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

		// Check for blocking agents
		if (!owner->IsBlocked()) {
			const FedoraAgent* blocking = controller->FindAgentInAgentPath(agent, targetPos);
			float blockingRadius = 6.0f;
			if (blocking != nullptr && glm::length2(blocking->ObjectTransform.Position - agent->ObjectTransform.Position) < blockingRadius * blockingRadius) {
				owner->SetBlocked(true);
			}
		}

		// If Path Blocked, Throw To Nearest Teammate
		if (owner->IsBlocked()) {
			const Team* team = agent->GetTeam();
			Vector3 throwToPos;
			FedoraAgent* passToAgent = team->FindPassToAgent(agent, throwToPos);

			if (passToAgent) {
				if (owner->FaceTowards(throwToPos, 9.5f)) {
					owner->ThrowFrisbee(agent);
					owner->GetFSM().ChangeState(AgentAITeamStates::Wait::Instance());

					PursueSignal signal;
					passToAgent->OnEvent(nullptr, signal);
				}
				else {
					owner->MoveAndAvoidEnemies(throwToPos);
				}
				return;
			}
		}

		// Actually Move to Goal
		float facingSpeed = 3.5f;
		// TODO: Face actual direction moved
		Vector3 facingPos = targetPos;
		float dotThreshold = 0.4f; // Min limit before facing moving direction
		Vector3 toTarget = glm::normalize(targetPos - agent->ObjectTransform.Position);

		if (glm::dot(toTarget, agent->GetDirection()) < dotThreshold) {
			facingPos = agent->ObjectTransform.Position + agent->GetDirection();
		}

		owner->FaceTowards(facingPos, facingSpeed); 
		owner->MoveAndAvoidEnemies(targetPos);

	}
	void MoveToScore::Exit(FedoraAgentInputAI* owner)
	{
		owner->SetBlocked(false);
	}
};