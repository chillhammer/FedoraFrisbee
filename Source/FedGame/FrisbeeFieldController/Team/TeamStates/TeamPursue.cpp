#include <FedPCH.h>
#include <Game/GameManager.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include <EventSystem/Events/TeamSignal.h>
#include "TeamStates.h"
/**
	Each state runs code that determines Fedora, the hat, behavior
**/
namespace Fed::TeamStates
{
	// Get control of fedora
	void Pursue::Enter(Team* owner)
	{
		owner->SetPursuitAgent(owner->FindClosesetAgentToFedora());
		PursueSignal signal;
		owner->GetPursuitAgent()->OnEvent(nullptr, signal); // Make this Agent Pursue
	}
	void Pursue::Execute(Team* owner)
	{
	}
	void Pursue::Exit(Team* owner)
	{
		owner->SetPursuitAgent(nullptr);
	}
}