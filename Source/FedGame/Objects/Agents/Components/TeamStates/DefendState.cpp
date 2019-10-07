#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include "FedoraAgentInputAITeamStates.h"
/**
	Each state runs code that determines Fedora, the hat, behavior
**/
namespace Fed::AgentAITeamStates
{
	// Doing nothing
	void Defend::Enter(FedoraAgentInputAI* owner)
	{
	}
	void Defend::Execute(FedoraAgentInputAI* owner)
	{
		FedoraAgent* agent = owner->GetOwner();
		FrisbeeFieldController* fieldController = agent->GetFieldController();
		Vector3 frisbeePos = fieldController->GetFedoraPosition();
		Vector3 targetGoalPos = fieldController->GetCourt()->GetGoalPosition(fieldController->GetEnemyTeam(agent->GetTeam()));
		float agentZ = agent->ObjectTransform.Position.z;

		// Face Frisbee
		float facingSpeed = 55.5f;
		if (!agent->GetHasFedora())
			owner->FaceTowards(frisbeePos, facingSpeed);

		// Chase if close
		float reactBorder = 14.0f;
		float goalSign = glm::sign(targetGoalPos.z - agentZ);
		// React if within react border or behind agent
		if (glm::abs(frisbeePos.z - agentZ) < reactBorder || glm::sign(frisbeePos.z - agentZ) != goalSign) {

			FedoraAgent* agentToStealFrom = fieldController->GetEnemyTeam(agent->GetTeam())->GetAgentWithFedora();

			// Pursue fedora, or steal from agent
			if (agentToStealFrom == nullptr) {
				owner->GetFSM().ChangeState(AgentAITeamStates::Pursue::Instance());
			}
			else {
				StealSignal signal(*agentToStealFrom);
				owner->OnEvent(nullptr, signal);
			}
		}
	}
	void Defend::Exit(FedoraAgentInputAI* owner)
	{
	}
}