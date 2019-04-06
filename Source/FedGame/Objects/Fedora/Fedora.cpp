#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include <Game/GameManager.h>
#include <Resources/ResourceManager.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include "FedoraStates.h"
#include "Fedora.h"

namespace Fed
{
	Fedora::Fedora() 
		:	GameObject("Fedora"), m_StateMachine(this, FedoraStates::Flying::Instance()),
			m_LaunchSpeed(20.f), m_AirResistance(10.f), m_TimeTilSlowdown(0.5f), m_BaseSpinSpeed(60.f), m_DropSpeed(1.2f), 
			m_Owner(nullptr), m_Speed(0.f)
	{
		SetBoundingBox(Vector3(0, 0, 0.35f), Vector3(0.4, 0.25f, 0.4));
	}
	// Handles Events given to it by agents
	void Fedora::OnEvent(const Subject * subject, Event & event)
	{
		Evnt::Dispatch<FrisbeeThrownEvent>(event, EVENT_BIND_FN(Fedora, OnFrisbeeThrown));
		Evnt::Dispatch<FrisbeePickupEvent>(event, EVENT_BIND_FN(Fedora, OnFrisbeePickup));
	}

	void Fedora::Update()
	{
		m_StateMachine.Update();
	}
	// Returns pointer to owner of fedora
	const FedoraAgent * Fedora::GetOwner() const
	{
		return m_Owner;
	}
	// Last thrown position updates when it is thrown
	const Vector3 Fedora::GetLastThrownPosition() const
	{
		return m_LastThrownPosition;
	}
	// Retursn units that the fedora will reach from throw range
	const float Fedora::GetThrowRange() const
	{
		return 30.f;
	}
	// Returns unit length direction fedora is moving towards
	const Vector3 Fedora::GetDirection() const
	{
		return m_Direction;
	}
	const float Fedora::GetSpeed() const
	{
		return m_Speed;
	}
	const float Fedora::GetLaunchSpeed() const
	{
		return m_LaunchSpeed;
	}
	void Fedora::SetFieldControllerReference(FrisbeeFieldController * fieldController)
	{
		m_FieldController = fieldController;
	}
	// Returns whether fedora has non-zero speed
	const bool Fedora::IsMoving() const
	{
		return m_Speed > 0.f;
	}
	// Calculates future position. Only when moving & considers max range
	const Vector3 Fedora::GetFuturePosition(float time, bool debugDraw) const
	{
		if (!IsMoving())
			return ObjectTransform.GetGlobalPosition();
		Vector3 futurePos = ObjectTransform.Position + m_Speed * m_Direction * time;
		// Caps at throw range
		if (glm::length(futurePos - GetLastThrownPosition()) > GetThrowRange())
		{
			futurePos = GetLastThrownPosition() + GetThrowRange() * m_Direction;
			futurePos.y = ObjectTransform.Position.y;
		}

		// Debug Draw ////
		if (debugDraw)
		{
			Matrix4x4 mat(1.f);
			mat = glm::translate(mat, futurePos - ObjectTransform.Position);
			Matrix4x4 current = ObjectTransform.GetMatrix();
			mat = mat * current;
			m_Model->Draw(Resources.GetShader("Debug"), mat);
		}
		//////////////////

		return futurePos;
	}
	// Reassigns to another owner
	void Fedora::SetOwner(const FedoraAgent * owner)
	{
		if (m_Owner == owner)
			return;
		m_Owner = owner;
		AttachToParent(nullptr);
		if (owner != nullptr)
		{
			m_Speed = 0.f;
			m_StateMachine.ChangeState(FedoraStates::Attached::Instance());
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
		// Collision
		
		if (m_FieldController) {
			std::vector<const GameObject*> walls = m_FieldController->GetCourt()->GetCollidingWalls(*this);
			if (!walls.empty())
			{
				ObjectTransform.Position -= m_Direction * m_Speed * Game.DeltaTime();
				Stop();
				m_TimeSinceThrown += 1;
			}
		}
	}
	// Stop movement. Collision or game reset
	void Fedora::Stop()
	{
		m_TimeSinceThrown = m_TimeTilSlowdown;
		m_Speed = 0;
	}
	bool Fedora::OnFrisbeeThrown(FrisbeeThrownEvent & e)
	{	
		m_TimeSinceThrown = 0;
		m_LastThrownPosition = e.GetPosition();
		SetOwner(nullptr);
		m_Speed = m_LaunchSpeed;
		m_Direction = e.GetDirection();
		m_StateMachine.ChangeState(FedoraStates::Flying::Instance());
		return false;
	}
	bool Fedora::OnFrisbeePickup(FrisbeePickupEvent & e)
	{
		SetOwner(&(e.GetAgent()));
		m_StateMachine.ChangeState(FedoraStates::Attached::Instance());
		return false;
	}
}