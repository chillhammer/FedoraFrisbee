#pragma once
#include "Fedora.h"
namespace Fed::FedoraStates
{
	// Fedora being worn
	STATE_CLASS_SINGLETON(Fedora, Attached,
		float m_TimeSinceAttached;
	);
	STATE_CLASS_SINGLETON(Fedora, Flying, );

}