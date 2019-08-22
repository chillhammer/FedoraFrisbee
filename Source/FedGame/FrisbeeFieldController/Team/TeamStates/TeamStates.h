#pragma once
#pragma once
#include "../Team.h"
namespace Fed::TeamStates
{
	// Fedora being worn
	STATE_CLASS_SINGLETON(Team, Standoff, );	// Nothing
	STATE_CLASS_SINGLETON(Team, Pursue, );		// Chase/Intercept Free Fedora
	STATE_CLASS_SINGLETON(Team, Score, );		// Try to score
	STATE_CLASS_SINGLETON(Team, Defend, );		// Try to defend
}