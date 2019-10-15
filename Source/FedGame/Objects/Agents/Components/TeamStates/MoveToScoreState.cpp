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
		// This cornered flag means they were cornered by the enemy. Thus ignore enemies since you can't do anything
		owner->SetCornered(false);
	}
	void MoveToScore::Execute(FedoraAgentInputAI* owner)
	{
		FedoraAgent* agent = owner->GetOwner();
		Vector3 agentPos = agent->ObjectTransform.Position;
		FrisbeeFieldController* controller = agent->GetFieldController();
		const std::vector<GoalTrigger>& goals = controller->GetCourt()->GetGoals();
		TeamColor color = agent->GetTeam()->GetColor();
		const GoalTrigger& targetGoal = (color != goals[0].Color ? goals[0] : goals[1]);
		Team* team = agent->GetTeam();
		const Team* enemyTeam = controller->GetEnemyTeam(team);
		

		ASSERT(targetGoal.Object.ObjectTransform.Position == Vector3(0, 0, 0), "Assumption that goals' position has no offset is false.");
		Vector3 targetPos = targetGoal.Object.GetBoundingBox().GetCenter(); targetPos.y = 0;
		Vector3 toTarget = targetPos - agentPos;

		// Check for blocking agents
		if (!owner->IsBlocked()) {
			// Someone in my path
			const FedoraAgent* blocking = controller->FindAgentInAgentPath(agent, targetPos);
			float blockingRadius = 5.0f;
			if (blocking != nullptr && glm::length2(blocking->ObjectTransform.Position - agent->ObjectTransform.Position) < blockingRadius * blockingRadius) {
				owner->SetBlocked(true);
			}
			// Close enemy in front of me
			const FedoraAgent* closest = enemyTeam->FindClosesetAgentToFedora();
			Vector3 closestPos = closest->ObjectTransform.Position;
			const FedoraAgent* closestToTarget = enemyTeam->FindClosestAgent(targetPos);
			float reactRadius = 2.0f;
			if (glm::length2(closestPos - agentPos) < reactRadius * reactRadius) {
				if (glm::dot(targetPos - agentPos, closestPos - agentPos) > 0.0f
					|| glm::length2(toTarget) > glm::length2(closestToTarget->ObjectTransform.Position - targetPos))
					owner->SetBlocked(true);
			}
		}

		// Check for cornered state
		if (!owner->IsCornered()) {
			if (controller->GetCourt()->IsPointWithinCorner(agent->ObjectTransform.Position)) {
				owner->SetCornered(true);
				LOG("Agent is cornered! ID: {0}", owner->GetOwner()->GetID());
			}
		}

		// If Path Blocked, Throw To Nearest Teammate
		if (owner->IsBlocked()) {
			Vector3 throwToPos;
			FedoraAgent* passToAgent = team->FindPassToAgent(agent, throwToPos);

			if (passToAgent != nullptr) {
				ASSERT(throwToPos != Vector3(0.0f, 0.0f, 0.0f), "Error in calculating throwToPos");

				if (owner->FaceTowards(throwToPos, 15.5f)) {
					owner->ThrowFrisbee(agent);
					owner->GetFSM().ChangeState(AgentAITeamStates::Wait::Instance());

					PursueSignal signal;
					passToAgent->OnEvent(nullptr, signal);
				}
				else {
					float dangerRadius = 2.0f;
					if (enemyTeam->CalculateDistSqrToClosestAgent(agentPos) < dangerRadius * dangerRadius) {
						if (!owner->IsCornered())
							owner->MoveAndAvoidEnemies(throwToPos);
						else
							owner->MoveTowards(throwToPos);
					}
				}
				return;
			}
		}

		// Actually Move to Goal
		float facingSpeed = 3.5f;
		Vector3 facingPos = targetPos;
		float dotThreshold = 0.4f; // Min limit before facing moving direction
		Vector3 toTargetDir = glm::normalize(toTarget);

		// Face to moving direction if threshhold surpassed
		if (glm::dot(toTargetDir, agent->GetDirection()) < dotThreshold) {
			facingPos = agent->ObjectTransform.Position + agent->GetDirection();
		}

		owner->FaceTowards(facingPos, facingSpeed); 
		if (!owner->IsCornered())
			owner->MoveAndAvoidEnemies(targetPos);
		else
			owner->MoveTowards(targetPos);

		// Blocking Check to see if moving backwards
		// If moving backwards
		if (glm::dot(targetPos - agentPos, owner->GetDirection()) <= -0.5f && !owner->IsBlocked())
			owner->SetBlocked(true);

	}
	void MoveToScore::Exit(FedoraAgentInputAI* owner)
	{
		owner->SetBlocked(false);
	}
};