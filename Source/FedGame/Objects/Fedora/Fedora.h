#pragma once
#include <GameObject/GameObject.h>

namespace Fed
{
	/**
		Frisbee-like object players pass around & wear.
	**/
	class Fedora : public GameObject
	{
	public:
		Fedora();
		void SetOwner(class FedoraAgent* owner);
	private:
		class FedoraAgent* m_Owner;
	};
}