#pragma once
#include "../FedoraAgentInputAI.h"
/**
	These states are primarily used for testing
**/
namespace Fed::AgentAIStates
{
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, GlobalMovement, );
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, Wait, );
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, Intercept, );
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, ChaseFrisbee, );
}