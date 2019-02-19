#pragma once
#include <vector>

namespace Fed
{
	enum TeamColor {Red, Blue};
	// Team controller. Overwatch AI that controls players
	class Team
	{
	public:
		Team();
		TeamColor GetColor() const;
		bool HasAgent(int agentID) const;
	private:
		std::vector<class FedoraAgent*> m_Agents;
		TeamColor m_Color;
		class FedoraAgent* m_ControllingAgent;
	};
}