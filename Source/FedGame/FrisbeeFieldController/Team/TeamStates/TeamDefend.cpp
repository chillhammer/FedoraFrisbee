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
	// Tries to retrieve the fedora from the enemy
	void Defend::Enter(Team* owner)
	{
		DefendSignal signal;
		owner->BroadcastSignal(signal);
	}
	void Defend::Execute(Team* owner)
	{
	}
	void Defend::Exit(Team* owner)
	{
	}
}