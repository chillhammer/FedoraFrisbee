#pragma once
#include <EventSystem/IObserver.h>
#include <EventSystem/Events/FrisbeeFieldEvent.h>
#include <StateMachine/StateMachine.h>
#include <EventSystem/Events/TeamSignal.h>
#include "FedoraAgentInputComponent.h"

namespace Fed
{
	class FedoraAgentInputAI
		: public FedoraAgentInputComponent
	{
	public:
		FedoraAgentInputAI();
		void Update(FedoraAgent* owner) override;
		bool MoveTowards(const Vector3& point);
		bool FaceTowards(const Vector3& point, float speed);
		void MoveBasedOnVelocity();
		Vector3 GetAgentPredictedPosition(const FedoraAgent* agent) const;
		FedoraAgent* GetOwner() const;
		bool	CanInterceptFedora();
		Vector3 GetInterceptPosition() const;
		StateMachine<FedoraAgentInputAI>& GetFSM();
		virtual void OnEvent(const Subject* subject, class Event& event) override;
		bool OnWaitSignal(WaitSignal& e);
		bool OnPursueSignal(PursueSignal& e);
		bool OnScoreSignal(ScoreSignal& e);
		bool OnDefendSignal(DefendSignal& e);
	private:
		StateMachine<FedoraAgentInputAI> m_StateMachine;
		FedoraAgent* m_Owner;
		Vector3 m_InterceptPosition;
		bool m_Accelerate;
		float m_Acceleration;
		float m_Friction;
	};
}