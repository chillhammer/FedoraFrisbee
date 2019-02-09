#include <FedPCH.h>
#include <Game/GameManager.h>
#include "FedoraAgentInputAIStates.h"
/**
	Each state runs code that determines Fedora, the hat, behavior
**/
namespace Fed::AgentAIStates
{
	// Doing nothing
	void Wait::Enter(FedoraAgentInputAI* owner)
	{
	}
	void Wait::Execute(FedoraAgentInputAI* owner)
	{
	}
	void Wait::Exit(FedoraAgentInputAI* owner)
	{
	}

	// Trying to intercept fedora
	void Intercept::Enter(FedoraAgentInputAI* owner)
	{

	}
	void Intercept::Execute(FedoraAgentInputAI* owner)
	{

	}
	void Intercept::Exit(FedoraAgentInputAI* owner)
	{
	}
}