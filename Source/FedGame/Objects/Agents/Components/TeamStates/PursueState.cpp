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
	void Pursue::Enter(FedoraAgentInputAI* owner)
	{
	}
	void Pursue::Execute(FedoraAgentInputAI* owner)
	{
		FedoraAgent* agent = owner->GetOwner();
		FrisbeeFieldController* fieldController = agent->GetFieldController();

	}
	void Pursue::Exit(FedoraAgentInputAI* owner)
	{
	}
}