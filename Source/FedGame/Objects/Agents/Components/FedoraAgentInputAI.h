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
		virtual ~FedoraAgentInputAI();
		void Update(FedoraAgent* owner) override;
		bool MoveTowards(const Vector3& point);
		bool MoveAndAvoidEnemies(const Vector3& point);
		bool MoveToSteal();
		bool FaceTowards(const Vector3& point, float speed);
		void FaceRotate(float speed);
		void MoveBasedOnVelocity();
		void SetBlocked(bool blocked);
		void SetCornered(bool cornered);
		FedoraAgent* GetOwner() const;
		bool	CanInterceptFedora();
		Vector3 GetInterceptPosition() const;
		Vector3 GetDirection() const;
		bool	IsStunned() const;
		bool	IsBlocked() const;
		bool	IsCornered() const;
		FedoraAgent* GetTargetAgent() const;
		StateMachine<FedoraAgentInputAI>& GetFSM();
		virtual void OnEvent(const Subject* subject, class Event& event) override;
		bool OnWaitSignal(WaitSignal& e);
		bool OnPursueSignal(PursueSignal& e);
		bool OnScoreSignal(ScoreSignal& e);
		bool OnDefendSignal(DefendSignal& e);
		bool OnStealSignal(StealSignal& e);
		bool OnStunSignal(StunSignal& e);
		bool OnAssistScoreSignal(AssistScoreSignal& e);
	private:
		StateMachine<FedoraAgentInputAI> m_StateMachine;
		FedoraAgent* m_Owner;
		FedoraAgent* m_TargetAgent; // Steal fedora from
		Vector3 m_InterceptPosition;
		bool m_Accelerate;
		float m_Acceleration;
		float m_Friction;
		float m_StunTimer;
		bool m_Blocked;
		bool m_Cornered;
	};
}