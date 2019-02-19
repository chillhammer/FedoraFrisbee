#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include "Team.h"

namespace Fed
{
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