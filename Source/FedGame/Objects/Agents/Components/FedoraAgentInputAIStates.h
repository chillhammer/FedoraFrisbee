#pragma once
#include "FedoraAgentInputAI.h"
namespace Fed::AgentAIStates
{
	// Fedora being worn
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, GlobalMovement, );
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, Wait, );
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, Intercept, );
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, ChaseFrisbee, );
}