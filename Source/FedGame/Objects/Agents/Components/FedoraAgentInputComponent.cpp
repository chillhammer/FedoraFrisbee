#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include "FedoraAgentInputComponent.h"

namespace Fed
{
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
			}
		}
	}
	void FedoraAgentInputComponent::OnEvent(const Subject* subject, Event& event)
	{
	}
}