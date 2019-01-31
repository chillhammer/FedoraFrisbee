#pragma once
#include <GameObject/GameObject.h>

namespace Fed
{
	class FedoraAgent 
		: public GameObject
	{
	public:
		FedoraAgent();
		FedoraAgent(Vector3 position);
	};
}