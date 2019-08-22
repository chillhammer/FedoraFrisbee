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
	// Tries to get the fedora into the goal
	void Score::Enter(Team* owner)
	{
		// Initial Logic is in  void Team::SetTeamPlay(TeamPlay play, FedoraAgent* agentWithFedora)
	}
	void Score::Execute(Team* owner)
	{
	}
	void Score::Exit(Team* owner)
	{
		owner->SetPursuitAgent(nullptr);
	}
}