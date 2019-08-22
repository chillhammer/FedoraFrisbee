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
	// Movement - common behavior across multiple states
	void GlobalMovement::Enter(FedoraAgentInputAI* owner)
	{
	}
	void GlobalMovement::Execute(FedoraAgentInputAI* owner)
	{
		owner->MoveBasedOnVelocity();
	}
	void GlobalMovement::Exit(FedoraAgentInputAI* owner)
	{
	}
}