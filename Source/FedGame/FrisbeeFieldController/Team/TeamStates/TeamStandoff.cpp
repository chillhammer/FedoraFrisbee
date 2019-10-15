#include <FedPCH.h>
#include <Game/GameManager.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include <EventSystem/Events/TeamSignal.h>
#include <Input/InputManager.h>
#include <Resources/ResourceManager.h>
#include "TeamStates.h"
/**
	Each state runs code that determines Fedora, the hat, behavior
**/
namespace Fed::TeamStates
{
	// Doing nothing
	void Standoff::Enter(Team* owner)
	{
		WaitSignal waitSignal;
		owner->BroadcastSignal(waitSignal);
	}
	void Standoff::Execute(Team* owner)
	{

		if (Input.IsKeyDown(KEY_W)) {
			owner->ExitStandoff();
		}
	}
	void Standoff::Exit(Team* owner)
	{
	}
}