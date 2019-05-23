#pragma once
#include "EventSystem/IObserver.h"

namespace Fed
{
	/**
		Interface for components to modify behavior.
		Used to switch between player input and AI input.
		Shares common behavior
	**/
	class FedoraAgentInputComponent : IObserver
	{
	public:
		FedoraAgentInputComponent() {}
		virtual void Update(class FedoraAgent* owner) = 0;
		void ThrowFrisbee(class FedoraAgent* owner);
		virtual void OnEvent(const Subject* subject, class Event& event) override;
	};
}