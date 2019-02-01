#pragma once

namespace Fed
{
	/**
		Interface for components to modify behavior.
		Used to switch between player input and AI input.
	**/
	class FedoraAgentInputComponent
	{
	public:
		FedoraAgentInputComponent() {}
		virtual void Update(class FedoraAgent* owner) = 0;
	};
}