#pragma once
#include <EventSystem/IObserver.h>
#include <EventSystem/Events/KeyEvent.h>
#include <EventSystem/Events/MouseEvent.h>
#include "FedoraAgentInputComponent.h"

namespace Fed
{
	class FedoraAgentInputPlayer
		: public FedoraAgentInputComponent, public IObserver
	{
	public:
		FedoraAgentInputPlayer();
		void OnEvent(const Subject* subject, Event& e);
		void Update(FedoraAgent* owner) override;
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
	private:
		bool m_InputFedoraThrow = false;
	};
}