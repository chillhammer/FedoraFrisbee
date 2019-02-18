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
	private:
		std::vector<class FedoraAgent*> m_Agents;
	};
}