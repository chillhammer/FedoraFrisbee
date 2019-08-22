#pragma once
#include "../FedoraAgentInputAI.h"
/**
	These states are primarily used for teamplay
**/
namespace Fed::AgentAITeamStates
{
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, Wait, );
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, Pursue, );
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, ChaseFrisbee, );
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, Intercept, );
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, MoveToScore, );
	STATE_CLASS_SINGLETON(FedoraAgentInputAI, GlobalMovement, );
}