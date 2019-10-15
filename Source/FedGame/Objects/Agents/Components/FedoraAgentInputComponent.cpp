#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include <Audio/AudioEngine.h>
#include "FedoraAgentInputComponent.h"

namespace Fed
{
	FedoraAgentInputComponent::~FedoraAgentInputComponent()
	{
	}
	// Attempts to throw frisbee from agent
	void FedoraAgentInputComponent::ThrowFrisbee(FedoraAgent * owner)
	{
		if (owner->GetHasFedora())
		{
			FrisbeeFieldController* fieldController = owner->GetFieldController();
			if (fieldController)
			{
				Vector3 throwDirection = owner->ObjectTransform.GetHeading();
				throwDirection.y = 0;
				throwDirection = glm::normalize(throwDirection);
				FrisbeeThrownEvent e(throwDirection, owner->ObjectTransform.Position, *owner);
				fieldController->FrisbeeThrown.Notify(e);
				Audio.PlaySound3D("Pass.wav", owner->ObjectTransform.Position);
			}
		}
	}
	void FedoraAgentInputComponent::OnEvent(const Subject* subject, Event& event)
	{
	}
}