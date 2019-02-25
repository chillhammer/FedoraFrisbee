#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include "TeamStates/TeamStates.h"
#include "Team.h"

namespace Fed
{
	Team::Team() : m_StateMachine(this, TeamStates::Standoff::Instance())
	{
	}
	TeamColor Team::GetColor() const
	{
		return m_Color;
	}
	bool Team::HasAgent(int agentID) const
	{
		for (auto it : m_Agents)
		{
			if (it->GetID() == agentID)
				return true;
		}
		return false;
	}
}