#pragma once
#include <GameObject/GameObject.h>
#include <StateMachine/StateMachine.h>
#include <EventSystem/IObserver.h>
#include <EventSystem/Events/FrisbeeFieldEvent.h>

namespace Fed
{
	/**
		Frisbee-like object players pass around & wear.
	**/
	class Fedora : public GameObject, public IObserver
	{
	public:
		Fedora();
		void OnEvent(const Subject* subject, Event& event);
		void Update();
		const class FedoraAgent* GetOwner() const;
		const Vector3 GetLastThrownPosition() const;
		const float GetThrowRange() const;
		const Vector3 GetDirection() const;
		const float GetSpeed() const;
		const float GetLaunchSpeed() const;
		void SetFieldControllerReference(class FrisbeeFieldController* fieldController);
		const bool IsMoving() const;
		const Vector3 GetFuturePosition(float time, bool debugDraw = false) const;
		const struct GoalTrigger* GetCollidingGoal() const;
		const class FrisbeeFieldController* GetFieldController() const;
		void SetOwner(const class FedoraAgent* owner);
		void SetCanScore(bool canScore);
		bool GetCanScore() const;
		void Move();
		void Stop();
	private:
		bool OnFrisbeeThrown(FrisbeeThrownEvent& e);
		bool OnFrisbeePickup(FrisbeePickupEvent& e);

		StateMachine<Fedora> m_StateMachine;
		const class FedoraAgent* m_Owner;
		const class FrisbeeFieldController* m_FieldController;
		float m_LaunchSpeed;
		float m_AirResistance;
		float m_TimeTilSlowdown;
		float m_BaseSpinSpeed;
		float m_DropSpeed;

		float m_Speed;
		Vector3 m_Direction;

		float m_TimeSinceThrown;
		Vector3 m_LastThrownPosition;
		bool m_CanScore;
	};
}