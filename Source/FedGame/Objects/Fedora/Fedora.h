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
		void SetOwner(class FedoraAgent* owner);
		void Move();
	private:
		bool OnFrisbeeThrown(FrisbeeThrownEvent& e);

		StateMachine<Fedora> m_StateMachine;
		class FedoraAgent* m_Owner;
		Vector3 m_WearingOffset;
		float m_LaunchSpeed;
		float m_AirResistance;
		float m_TimeTilSlowdown;
		float m_BaseSpinSpeed;
		float m_DropSpeed;

		float m_Speed;
		Vector3 m_Direction;

		float m_TimeSinceThrown;
	};
}