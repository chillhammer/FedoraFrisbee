#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include "Fedora.h"

namespace Fed
{
	Fedora::Fedora() : GameObject("Fedora")
	{

	}
	void Fedora::SetOwner(FedoraAgent * owner)
	{
		m_Owner = owner;
		AttachToParent(owner);
		ObjectTransform.Parent = &owner->ObjectTransform;
	}
}