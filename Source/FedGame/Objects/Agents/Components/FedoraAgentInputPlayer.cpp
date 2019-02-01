#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Game/GameManager.h>
#include "FedoraAgentInputPlayer.h"
#include "../FedoraAgent.h"

namespace Fed
{
	void FedoraAgentInputPlayer::Update(FedoraAgent * owner)
	{
		// Moves Camera
		if (owner->m_Camera)
		{
			owner->m_Camera->SetPivotPosition(owner->ObjectTransform.Position);
		}

		Vector3 moveDir(0, 0, 0);
		float moveSpeed = 1.1;
		if (Input.IsKeyDown(KEY_Y))
		{
			moveDir = owner->ObjectTransform.GetSide();
		}

		//if (Input)

		owner->ObjectTransform.Position += moveDir * 1.1f * Game.DeltaTime();
	}
}