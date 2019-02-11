#include <FedPCH.h>
#include <Game/GameManager.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
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
		FedoraAgent* agent = owner->GetOwner();
		if (agent->InFedoraPath())
		{
			//LOG("Agent {0} is in fedora path", agent->GetID());
		}
		else
		{
			if (owner->CanInterceptFedora())
			{
				owner->GetFSM().ChangeState(AgentAIStates::Intercept::Instance());
			}
		}
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
		FedoraAgent* agent = owner->GetOwner();
		/*if (agent->InFedoraPath())
		{
			owner->GetFSM().ChangeState(AgentAIStates::Wait::Instance());
		}
		else*/
		{
			Vector3 dir = owner->GetInterceptPosition() - agent->ObjectTransform.Position;
			if (glm::length(dir) < agent->GetMaxSpeed() * Game.DeltaTime())
			{
				agent->ObjectTransform.Position = owner->GetInterceptPosition();
				owner->GetFSM().ChangeState(AgentAIStates::Wait::Instance());
			}
			else
			{
				dir = glm::normalize(dir);
				agent->ObjectTransform.Position += dir * agent->GetMaxSpeed() * Game.DeltaTime();
			}
			
		}
	}
	void Intercept::Exit(FedoraAgentInputAI* owner)
	{
	}
}