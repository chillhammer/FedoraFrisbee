#include <FedPCH.h>
#include <Objects/Agents/FedoraAgent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include <Game/GameManager.h>
#include "TestStates/FedoraAgentInputAITestStates.h"
#include "TeamStates/FedoraAgentInputAITeamStates.h"
#include "FedoraAgentInputAI.h"

namespace Fed
{
	FedoraAgentInputAI::FedoraAgentInputAI() : m_StateMachine(this, AgentAITestStates::Wait::Instance()),
		m_Acceleration(12.f), m_Friction(-30.f), m_Accelerate(false), m_Owner(nullptr), m_InterceptPosition(0,0,0), 
		m_TargetAgent(nullptr), m_StunTimer(0.0f)
	{
		m_StateMachine.SetGlobalState(AgentAITeamStates::GlobalMovement::Instance());
	}
	// Handles Events
	void FedoraAgentInputAI::OnEvent(const Subject * subject, Event & e)
	{
		Evnt::Dispatch<WaitSignal>(e, EVENT_BIND_FN(FedoraAgentInputAI, OnWaitSignal));
		Evnt::Dispatch<PursueSignal>(e, EVENT_BIND_FN(FedoraAgentInputAI, OnPursueSignal));
		Evnt::Dispatch<ScoreSignal>(e, EVENT_BIND_FN(FedoraAgentInputAI, OnScoreSignal));
		Evnt::Dispatch<DefendSignal>(e, EVENT_BIND_FN(FedoraAgentInputAI, OnDefendSignal));
		Evnt::Dispatch<StealSignal>(e, EVENT_BIND_FN(FedoraAgentInputAI, OnStealSignal));
		Evnt::Dispatch<StunSignal>(e, EVENT_BIND_FN(FedoraAgentInputAI, OnStunSignal));
	}
	bool FedoraAgentInputAI::OnWaitSignal(WaitSignal& e)
	{
		m_StateMachine.ChangeState(AgentAITeamStates::Wait::Instance());
		return false;
	}
	bool FedoraAgentInputAI::OnPursueSignal(PursueSignal& e)
	{
		m_StateMachine.ChangeState(AgentAITeamStates::Pursue::Instance());
		return false;
	}
	bool FedoraAgentInputAI::OnScoreSignal(ScoreSignal& e)
	{
		m_StateMachine.ChangeState(AgentAITeamStates::MoveToScore::Instance());
		return false;
	}
	bool FedoraAgentInputAI::OnDefendSignal(DefendSignal& e)
	{
		return false;
	}
	bool FedoraAgentInputAI::OnStealSignal(StealSignal& e)
	{
		m_TargetAgent = &e.GetAgent();
		m_StateMachine.ChangeState(AgentAITeamStates::MoveToSteal::Instance());
		return false;
	}
	bool FedoraAgentInputAI::OnStunSignal(StunSignal& e)
	{
		m_StunTimer = e.GetTime();
		return false;
	}
	void FedoraAgentInputAI::Update(FedoraAgent * owner)
	{
		ASSERT(owner != nullptr, "Agent cannot be nullptr");
		// Stores owner for use within FSM
		m_Owner = owner;

		m_StateMachine.Update();

		m_StunTimer = glm::max(0.0f, m_StunTimer - Game.DeltaTime());
	}

	////////////////////////////////////
	// Functions to use with AI States
	/////////////////////////////////////

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
	// Move to point while avoiding all enemies
	bool FedoraAgentInputAI::MoveAndAvoidEnemies(const Vector3& point)
	{
		FedoraAgent* owner = GetOwner();
		Vector3 agentPos = owner->ObjectTransform.Position;
		Vector3 dir = glm::normalize(point - agentPos);
		float visionRadius = 7.0f;
		Vector3 avoidanceSteer = Vector3(0, 0, 0);
		float closestDist = -1;

		Team* enemyTeam = owner->GetFieldController()->GetEnemyTeam(owner->GetTeam());
		auto enemyPositions = enemyTeam->GetAgentPositions();

		for (const Vector3& ePos : enemyPositions) {
			Vector3 toEnemy = ePos - agentPos;
			float dot = glm::dot(dir, toEnemy);
			// Only consider enemies in front of you
			if (dot < 0)
				continue;
			float dist = glm::length(toEnemy);
			// Ignore if farther than vision radius
			if (dist > visionRadius)
				continue;

			// Only consider closest enemy agent
			if (closestDist == -1 || dist < closestDist) {
				Vector3 projected = dir * glm::dot(glm::normalize(toEnemy), dir) * dist;
				float distFromPath = glm::length(toEnemy - projected);
				float avoidanceRadius = 3.0f;
				// Ignore if farther than avoidance radius
				if (distFromPath >= avoidanceRadius)
					continue;
				// Will set avoidance steer in direction of enemy -> path (away from enemy) multiplied by how close they are
				avoidanceSteer = glm::normalize(projected - toEnemy) * (avoidanceRadius - distFromPath);
				closestDist = dist;
			}
		}
		bool result = MoveTowards(point);
		owner->m_Direction = glm::normalize(owner->m_Direction + avoidanceSteer);

		return result;
	}
	// Intercepts FedoraAgent with Fedora. Unit command for AI
	bool FedoraAgentInputAI::MoveToSteal()
	{
		ASSERT(m_TargetAgent && m_TargetAgent->GetHasFedora(), "TargetAgent must exist with fedora");
		FedoraAgent* owner = GetOwner();
		// Move towards agent future position to intercept them
		Vector3 targetPos = GetAgentPredictedPosition(m_TargetAgent, glm::max(0.1f, owner->m_Speed));
		Vector3 dir = targetPos - m_Owner->ObjectTransform.Position;
		m_Accelerate = true;
		owner->m_Direction = glm::normalize(dir);
		owner->m_Speed = glm::max(owner->m_Speed, owner->GetMaxSpeed() * 0.5f);
		// Keeps same format as other move functions, but accelerate into target
		return false;
	}
	// Lerp rotates towards point
	bool FedoraAgentInputAI::FaceTowards(const Vector3 & point, float speed)
	{
		FedoraAgent* owner = GetOwner();
		Vector3 dir = point - owner->ObjectTransform.Position; dir.y = 0;
		if (dir == Vector3(0, 0, 0))
			return true;
		dir = glm::normalize(dir);
		// (0, 0, 1) = 0 Yaw
		float targetYaw = glm::degrees(std::atan2(dir.z, dir.x)) - 90.f;

		owner->ObjectTransform.SetYaw(LerpAngle(owner->ObjectTransform.GetYaw(), targetYaw, speed * Game.DeltaTime()));

		// Snap if within neglible range
		if (glm::abs(AngleDiff(owner->ObjectTransform.GetYaw(), targetYaw)) < 3.f)
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

		m_Owner->m_PrevPosition = m_Owner->ObjectTransform.Position;
		m_Owner->ObjectTransform.Position += m_Owner->m_Direction * m_Owner->m_Speed * Game.DeltaTime();
		
		// Reset Accelertion
		m_Accelerate = false;

		Vector3 heading = m_Owner->ObjectTransform.GetHeading(); heading.y = 0; heading = glm::normalize(heading);
		float horLean = 1 - glm::abs(glm::dot(heading, m_Owner->m_Direction));
		float moveAngle = glm::degrees(std::atan2(m_Owner->m_Direction.z, m_Owner->m_Direction.x)) + 180;
		float moveAngleOffset = 180 - moveAngle;
		float headingAngle = ProcessAngle(glm::degrees(std::atan2(m_Owner->m_Direction.z, m_Owner->m_Direction.x)) + 180 + moveAngleOffset);
		horLean *= glm::sign(headingAngle - 180);

		// Lean based on speed
		float leanAmount = m_Owner->m_Speed / m_Owner->GetMaxSpeed();
		m_Owner->ObjectTransform.SetPitch(LerpAngle(m_Owner->ObjectTransform.GetPitch(), leanAmount * 20.f, 5.0f * Game.DeltaTime()));
		m_Owner->ObjectTransform.SetRoll(LerpAngle(m_Owner->ObjectTransform.GetRoll(), horLean * leanAmount * 150.0f, 5.0f * Game.DeltaTime()));
	}
	// Uses predicted future movement to throw fedora or to steal fedora
	Vector3 FedoraAgentInputAI::GetAgentPredictedPosition(const FedoraAgent * agent, float interceptingSpeed) const
	{
		ASSERT(agent != nullptr, "Agent cannot be nullptr");
		float dist = glm::length(GetOwner()->ObjectTransform.Position - agent->ObjectTransform.Position);
		float timeForFedora = dist / interceptingSpeed;
		Vector3 predictedAgentPos = agent->GetFuturePosition(timeForFedora);
		return predictedAgentPos;
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
	bool FedoraAgentInputAI::IsStunned() const
	{
		return m_StunTimer > 0;
	}
	FedoraAgent* FedoraAgentInputAI::GetTargetAgent() const
	{
		return m_TargetAgent;
	}
	// Provides access to state machine. Used from within state machine
	StateMachine<FedoraAgentInputAI>& FedoraAgentInputAI::GetFSM()
	{
		return m_StateMachine;
	}
}