#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Game/GameManager.h>
#include <EventSystem/Events/FrisbeeFieldEvent.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include "FedoraAgentInputPlayer.h"
#include <EventSystem/Events/TeamSignal.h>
#include "../FedoraAgent.h"

namespace Fed
{
	FedoraAgentInputPlayer::FedoraAgentInputPlayer()
	{
		Input.MouseClicked.AddObserver(this);
		Input.KeyPressed.AddObserver(this);
	}
	FedoraAgentInputPlayer::~FedoraAgentInputPlayer()
	{
		Input.MouseClicked.RemoveObserver(this);
		Input.KeyPressed.RemoveObserver(this);

	}
	// Handles events, primarily input
	void FedoraAgentInputPlayer::OnEvent(const Subject * subject, Event & e)
	{
		Evnt::Dispatch<KeyPressedEvent>(e, EVENT_BIND_FN(FedoraAgentInputPlayer, OnKeyPressed));
		Evnt::Dispatch<MouseButtonPressedEvent>(e, EVENT_BIND_FN(FedoraAgentInputPlayer, OnMousePressed));
	}
	void FedoraAgentInputPlayer::Update(FedoraAgent * owner)
	{
		// Setting Up Movement Variables
		Vector3 moveDir(0, 0, 0);
		float maxMoveSpeed = owner->GetMaxSpeed();

		float forwardSign	= (Input.IsKeyDown(KEY_W) ? 1 : 0) - (Input.IsKeyDown(KEY_S) ? 1 : 0);
		float sideSign	= (Input.IsKeyDown(KEY_D) ? 1 : 0) - (Input.IsKeyDown(KEY_A) ? 1 : 0);
		Vector3 heading = owner->ObjectTransform.GetHeading(); heading.y = 0; heading = glm::normalize(heading);
		Vector3 side	= owner->ObjectTransform.GetSide(); side.y = 0; side = glm::normalize(side);
		moveDir += heading	* forwardSign;
		moveDir += side		* sideSign;
		bool moved = glm::length(moveDir) != 0;
		if (moved) {
			moveDir = glm::normalize(moveDir);
			owner->m_Direction = glm::lerp(owner->m_Direction, moveDir, 0.5f);
			owner->m_PrevPosition = owner->ObjectTransform.Position;
		}
		// Accelerate / Friction - Speed
		float acceleration = (moved ? 12.5f : -15.f);
		float speed = owner->m_Speed;
		speed = (speed < 3.f && forwardSign == 1 ? 3.f : (speed < 1.5f && moved ? 1.5f : speed));
		float multiplier = (forwardSign == 1.f || owner->m_Camera->Mode != CameraMode::Pivot ? 1.0f : 1.0f);
		speed = glm::min(speed + acceleration * Game.DeltaTime(), maxMoveSpeed * multiplier);
		speed = (speed < 0 ? 0 : speed);
		owner->m_Speed = speed;

		// Actual Movement
		owner->ObjectTransform.Position += owner->m_Speed * owner->m_Direction * Game.DeltaTime();


		// Rotates Agent
		if (owner->m_Camera && owner->m_Camera->Mode == CameraMode::Pivot)
		{
			float cameraYaw = owner->m_Camera->ObjectTransform.GetYaw();
			owner->ObjectTransform.SetYaw(cameraYaw);
		}

		float forwardLean = 2.f + (owner->IsInvincible() ? 1.0f : 0.0f);
		float sideLean = 0.9f;
		float leanSpeed = 20.f;
		owner->ObjectTransform.SetPitch(LerpAngle(owner->ObjectTransform.GetPitch(), forwardSign * owner->m_Speed * forwardLean, leanSpeed * Game.DeltaTime()));
		owner->ObjectTransform.SetRoll(LerpAngle(owner->ObjectTransform.GetRoll(), sideSign * owner->m_Speed * sideLean, leanSpeed * Game.DeltaTime()));
	}
	// Post collision update
	void FedoraAgentInputPlayer::LateUpdate(FedoraAgent* owner)
	{
		// Update Camera Pivot
		if (owner->m_Camera)
		{
			owner->m_Camera->SetPivotPosition(owner->ObjectTransform.Position);
		}

		// Input Trigger Fedora Throwing Event
		if (m_InputFedoraThrow)
		{
			m_InputFedoraThrow = false;

			if (owner->GetHasFedora()) {
				FedoraAgent* bestReceiver = owner->m_Team->FindClosestAgentToRay(owner->ObjectTransform.Position, owner->ObjectTransform.GetHeading(), owner);
				PursueSignal signal;
				if (bestReceiver)
					bestReceiver->OnEvent(nullptr, signal);
			}

			ThrowFrisbee(owner);
		}
	}
	bool FedoraAgentInputPlayer::OnKeyPressed(KeyPressedEvent & e)
	{
		switch (e.GetKeyCode())
		{
			// In case I decide to implement more actions
		}
		return false;
	}
	bool FedoraAgentInputPlayer::OnMousePressed(MouseButtonPressedEvent & e)
	{
		if (e.GetMouseButton() == 0)
		{
			m_InputFedoraThrow = true;
		}
		return false;
	}
}