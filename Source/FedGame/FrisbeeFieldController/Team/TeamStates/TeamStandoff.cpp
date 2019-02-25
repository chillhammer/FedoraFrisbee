#include <FedPCH.h>
#include <Game/GameManager.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include "TeamStates.h"
/**
	Each state runs code that determines Fedora, the hat, behavior
**/
namespace Fed::TeamStates
{
	// Doing nothing
	void Standoff::Enter(Team* owner)
	{
	}
	void Standoff::Execute(Team* owner)
	{
	}
	void Standoff::Exit(Team* owner)
	{
	}
}