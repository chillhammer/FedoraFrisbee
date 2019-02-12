#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include <Game/GameManager.h>
#include "FedoraAgentInputAIStates.h"
#include "FedoraAgentInputAI.h"

namespace Fed
{
	Fed::FedoraAgentInputAI::FedoraAgentInputAI() : m_StateMachine(this, AgentAIStates::Wait::Instance()),
		m_Acceleration(12.f), m_Friction(-30.f)
	{
		m_StateMachine.SetGlobalState(AgentAIStates::GlobalMovement::Instance());
	}
	// Handles Events
	void FedoraAgentInputAI::OnEvent(const Subject * subject, Event & e)
	{
	}
	void FedoraAgentInputAI::Update(FedoraAgent * owner)
	{
		ASSERT(owner != nullptr, "Agent cannot be nullptr");
		// Stores owner for use within FSM
		m_Owner = owner;

		m_StateMachine.Update();
	}
	// Approaches point. Unit movement command for AI
	bool FedoraAgentInputAI::MoveTowards(const Vector3 & point)
	{
		FedoraAgent* owner = GetOwner();
		Vector3 dir = point - owner->ObjectTransform.Position;
		// How much time at the current speed would decelerating bring speed to 0
		float timeToStop = owner->m_Speed / m_Friction * -1.f;
		// Integrate to find out how much distance friction would carry you at current speed
		// speed * x - 1/2 * friction x ^ 2
		float distToStop = owner->m_Speed * timeToStop - (0.5f * -m_Friction * timeToStop * timeToStop);
		
		if (glm::length(dir) <= distToStop)
		{
			//owner->ObjectTransform.Position = point;
			m_Accelerate = false;
			return true;
		}
		else
		{
			owner->m_Direction = glm::normalize(dir);
			owner->m_Speed = glm::max(owner->m_Speed, owner->GetMaxSpeed() * 0.5f);
			m_Accelerate = true;
			return false;
		}
	}
	// Lerp rotates towards point
	bool FedoraAgentInputAI::FaceTowards(const Vector3 & point, float speed)
	{
		FedoraAgent* owner = GetOwner();
		Vector3 dir = point - owner->ObjectTransform.Position; dir.y = 0;
		dir = glm::normalize(dir);
		// (0, 0, 1) = 0 Yaw
		float targetYaw = glm::degrees(std::atan2(dir.z, dir.x)) - 90.f;

		owner->ObjectTransform.SetYaw(LerpAngle(owner->ObjectTransform.GetYaw(), targetYaw, speed * Game.DeltaTime()));

		// Snap if within neglible range
		if (glm::abs(owner->ObjectTransform.GetYaw() - targetYaw) < 0.1f)
		{
			owner->ObjectTransform.SetYaw(targetYaw);
			return true;
		}

		return false;
	}
	// Actually moves object based on speed and direction. Changes
	void FedoraAgentInputAI::MoveBasedOnVelocity()
	{
		// Accelerate / Slow down
		float acceleration = (m_Accelerate ? m_Acceleration : m_Friction) * Game.DeltaTime();
		m_Owner->m_Speed = glm::clamp(m_Owner->m_Speed + acceleration, 0.f, m_Owner->GetMaxSpeed());

		m_Owner->ObjectTransform.Position += m_Owner->m_Direction * m_Owner->m_Speed * Game.DeltaTime();
		// Reset Accelertion
		m_Accelerate = false;

		// Lean based on speed
		float leanAmount = m_Owner->m_Speed / m_Owner->GetMaxSpeed();
		m_Owner->ObjectTransform.SetPitch(LerpAngle(m_Owner->ObjectTransform.GetPitch(), leanAmount * 20.f, 5.0f * Game.DeltaTime()));
	}
	// Returns owner of this component. Used within FSM
	FedoraAgent * FedoraAgentInputAI::GetOwner() const
	{
		return m_Owner;
	}
	// Returns whether one can intercept fedora.
	// Also updates intercept position variable
	bool FedoraAgentInputAI::CanInterceptFedora()
	{
		return GetOwner()->GetFieldController()->CanAgentInterceptFedora(GetOwner(), &m_InterceptPosition);
	}
	// Get Intercept Position for Agent to Travel to. Flatten the Y
	Vector3 FedoraAgentInputAI::GetInterceptPosition() const
	{
		Vector3 pos = m_InterceptPosition;
		pos.y = 0;
		return pos;
	}
	// Provides access to state machine. Used from within state machine
	StateMachine<FedoraAgentInputAI>& FedoraAgentInputAI::GetFSM()
	{
		return m_StateMachine;
	}
}