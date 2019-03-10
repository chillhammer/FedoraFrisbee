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
	void Team::SetColor(TeamColor color)
	{
		m_Color = color;
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
	void Team::AddAgent(FedoraAgent * agent)
	{
		ASSERT(!HasAgent(agent->GetID()), "Can't add agent that exists");
		m_Agents.push_back(agent);
	}
	void Team::RemoveAgent(FedoraAgent * agent)
	{
		ASSERT(HasAgent(agent->GetID()), "Can't remove agent that doesn't exists");
		m_Agents.erase(std::remove(m_Agents.begin(), m_Agents.end(), agent), m_Agents.end());
	}
}