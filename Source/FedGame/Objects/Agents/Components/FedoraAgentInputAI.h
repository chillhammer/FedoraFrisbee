#pragma once
#include <EventSystem/IObserver.h>
#include <EventSystem/Events/FrisbeeFieldEvent.h>
#include <StateMachine/StateMachine.h>
#include "FedoraAgentInputComponent.h"

namespace Fed
{
	class FedoraAgentInputAI
		: public FedoraAgentInputComponent, public IObserver
	{
	public:
		FedoraAgentInputAI();
		void OnEvent(const Subject* subject, Event& e);
		void Update(FedoraAgent* owner) override;
		FedoraAgent* GetOwner() const;
		bool CanInterceptFedora();
		Vector3 GetInterceptPosition() const;
		StateMachine<FedoraAgentInputAI>& GetFSM();
	private:
		StateMachine<FedoraAgentInputAI> m_StateMachine;
		FedoraAgent* m_Owner;
		Vector3 m_InterceptPosition;
	};
}