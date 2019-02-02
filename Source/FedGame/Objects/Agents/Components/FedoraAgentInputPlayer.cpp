#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Game/GameManager.h>
#include "FedoraAgentInputPlayer.h"
#include "../FedoraAgent.h"

namespace Fed
{
	void FedoraAgentInputPlayer::Update(FedoraAgent * owner)
	{
		// Setting Up Movement Variables
		Vector3 moveDir(0, 0, 0);
		owner->m_MaxSpeed = 5.5f;
		float maxMoveSpeed = owner->m_MaxSpeed;

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
		}

		float acceleration = (moved ? 8.5f : -15.f);
		float speed = owner->m_Speed;
		speed = (speed < 3.f && forwardSign == 1 ? 3.f : (speed < 1.5f && moved ? 1.5f : speed));
		float multiplier = (forwardSign == 1 ? (sideSign == 0 ? 1.95f : 1.95f) : 1);
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
			float cameraPitch = owner->m_Camera->ObjectTransform.GetPitch();
			//owner->ObjectTransform.SetPitch(cameraPitch);
			// LOG("Camera Yaw: {0}", cameraYaw);
		}

		owner->ObjectTransform.SetPitch(forwardSign * 15.f);
		owner->ObjectTransform.SetRoll(sideSign * 15.f);

		// Update Camera Pivot
		if (owner->m_Camera)
		{
			owner->m_Camera->SetPivotPosition(owner->ObjectTransform.Position);
		}
	}
}