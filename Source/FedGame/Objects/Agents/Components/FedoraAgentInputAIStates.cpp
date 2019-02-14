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

		// Stare at player
		const FedoraAgent* player = agent->GetFieldController()->FindPlayerAgent();
		
		// Face Player
		Vector3 pointToFace = player->ObjectTransform.Position;
		if (agent->GetHasFedora())
		{
			pointToFace = owner->GetAgentPredictedPosition(player);
		}
		bool facingPlayer = owner->FaceTowards(pointToFace, 55.5f);

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
			else if (owner->GetOwner()->GetFieldController()->IsFedoraFree()
				&& owner->GetOwner()->GetFieldController()->GetLastThrownAgentID() != owner->GetOwner()->GetID())
			{
				owner->GetFSM().ChangeState(AgentAIStates::ChaseFrisbee::Instance());
			}
		}

		// Throw if possible
		if (facingPlayer)
		{
			owner->ThrowFrisbee(owner->GetOwner());
		}
	}
	void Wait::Exit(FedoraAgentInputAI* owner)
	{
	}
	//////////////////////////////////////////////////////////////
	// Trying to intercept fedora
	/////////////////////////////////////////////////////////////
	void Intercept::Enter(FedoraAgentInputAI* owner)
	{

	}
	void Intercept::Execute(FedoraAgentInputAI* owner)
	{
		FedoraAgent* agent = owner->GetOwner();
		owner->FaceTowards(owner->GetInterceptPosition(), 6.5f);
		if (owner->MoveTowards(owner->GetInterceptPosition()))
		{
			owner->GetFSM().ChangeState(AgentAIStates::Wait::Instance());
		}
	}
	void Intercept::Exit(FedoraAgentInputAI* owner)
	{
	}

	////////////////////////////////////////////////////////////////
	// Trying to run at fedora
	////////////////////////////////////////////////////////////////
	void ChaseFrisbee::Enter(FedoraAgentInputAI* owner)
	{

	}
	void ChaseFrisbee::Execute(FedoraAgentInputAI* owner)
	{
		FrisbeeFieldController* controller = owner->GetOwner()->GetFieldController();
		Vector3 frisbeePos = controller->GetFedoraPosition();
		frisbeePos.y = 0;
		owner->FaceTowards(frisbeePos, 6.5f);
		owner->MoveTowards(frisbeePos);

		// Stop chasing if it is picked up
		if (!controller->IsFedoraFree())
		{
			owner->GetFSM().ChangeState(AgentAIStates::Wait::Instance());
		}
	}
	void ChaseFrisbee::Exit(FedoraAgentInputAI* owner)
	{
	}

	// Regular movement - global state
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