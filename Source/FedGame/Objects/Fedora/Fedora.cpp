#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include <Game/GameManager.h>
#include "FedoraStates.h"
#include "Fedora.h"

namespace Fed
{
	Fedora::Fedora() 
		:	GameObject("Fedora"), m_WearingOffset(0, 1.92f, 0), m_StateMachine(this, FedoraStates::Attached::Instance()),
			m_LaunchSpeed(20.f), m_AirResistance(10.f), m_TimeTilSlowdown(0.5f), m_BaseSpinSpeed(60.f), m_DropSpeed(1.2f)
	{

	}
	// Handles Events given to it by agents
	void Fedora::OnEvent(const Subject * subject, Event & event)
	{
		Evnt::Dispatch<FrisbeeThrownEvent>(event, EVENT_BIND_FN(Fedora, OnFrisbeeThrown));
	}
	void Fedora::Update()
	{
		m_StateMachine.Update();
	}
	// Reassigns to another owner
	void Fedora::SetOwner(FedoraAgent * owner)
	{
		if (m_Owner == owner)
			return;
		if (m_Owner)
		{
			m_Owner->SetHasFedora(false);
		}
		m_Owner = owner;
		AttachToParent(owner); // Update transforms
		
		// Teleport Hat to Head
		if (owner)
		{
			owner->SetHasFedora(true);
			this->ObjectTransform.Position = m_WearingOffset;
		}
	}
	// Flying movement
	void Fedora::Move()
	{
		// Move based on speed
		ObjectTransform.Position += m_Direction * m_Speed * Game.DeltaTime();
		// Rotate
		ObjectTransform.SetYaw(ObjectTransform.GetYaw() + m_BaseSpinSpeed * m_Speed * Game.DeltaTime());
		ObjectTransform.SetRoll(LerpAngle(ObjectTransform.GetRoll(), 0, 0.1f));
		ObjectTransform.SetPitch(LerpAngle(ObjectTransform.GetPitch(), 0, 0.1f));
		// Activate Air Resistance & Drop
		m_TimeSinceThrown += Game.DeltaTime();
		if (m_TimeSinceThrown > m_TimeTilSlowdown)
		{
			float timeSinceSlow = (m_TimeSinceThrown - m_TimeTilSlowdown);
			m_Speed = glm::max(0.f, m_Speed - m_AirResistance * Game.DeltaTime());
			ObjectTransform.Position.y = glm::max(0.23f, ObjectTransform.Position.y - m_DropSpeed * timeSinceSlow * Game.DeltaTime());
		}
	}
	bool Fedora::OnFrisbeeThrown(FrisbeeThrownEvent & e)
	{	
		m_TimeSinceThrown = 0;
		SetOwner(nullptr);
		m_Speed = m_LaunchSpeed;
		m_Direction = e.GetDirection();
		m_StateMachine.ChangeState(FedoraStates::Flying::Instance());
		return false;
	}
}