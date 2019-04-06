#include <FedPCH.h>
#include <Game/GameManager.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include "FedoraAgentInputAITestStates.h"
/**
	Each state runs code that determines Fedora, the hat, behavior
**/
namespace Fed::AgentAITestStates
{
	// Doing nothing
	void Wait::Enter(FedoraAgentInputAI* owner)
	{
	}
	void Wait::Execute(FedoraAgentInputAI* owner)
	{
		FedoraAgent* agent = owner->GetOwner();
		FrisbeeFieldController* fieldController = agent->GetFieldController();

		// Get Player Object
		const FedoraAgent* player = agent->GetFieldController()->FindPlayerAgent();
		
		ASSERT(player != nullptr, "Player does not exist");

		// Face Player
		Vector3 pointToFace = player->ObjectTransform.Position;
		if (agent->GetHasFedora())
		{
			pointToFace = owner->GetAgentPredictedPosition(player);
		}
		bool facingPlayer = owner->FaceTowards(pointToFace, 55.5f);

		if (!agent->InFedoraPath())
		{
			// Try to intercept
			if (owner->CanInterceptFedora())
			{
				owner->GetFSM().ChangeState(Intercept::Instance());
			}
			// Otherwise chase
			else if (fieldController->IsFedoraFree()
				&& fieldController->GetAgentFromID(fieldController->GetLastThrownAgentID())->GetTeam() != agent->GetTeam())
			{
				owner->GetFSM().ChangeState(ChaseFrisbee::Instance());
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
			owner->GetFSM().ChangeState(Wait::Instance());
		}
		if (!agent->GetFieldController()->IsFedoraMoving())
		{
			owner->GetFSM().ChangeState(ChaseFrisbee::Instance());
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
			owner->GetFSM().ChangeState(Wait::Instance());
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