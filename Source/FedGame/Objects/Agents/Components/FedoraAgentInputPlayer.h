#pragma once
#include "FedoraAgentInputComponent.h"

namespace Fed
{
	class FedoraAgentInputPlayer
		: public FedoraAgentInputComponent
	{
	public:
		FedoraAgentInputPlayer() {}
		void Update(FedoraAgent* owner) override;
	};
}