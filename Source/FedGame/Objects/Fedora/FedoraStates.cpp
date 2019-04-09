#include <FedPCH.h>
#include <Game/GameManager.h>
#include "FedoraStates.h"
/**
	Each state runs code that determines Fedora, the hat, behavior
**/
namespace Fed::FedoraStates
{
	// Attached State, fedora is worn
	void Attached::Enter(Fedora* owner)
	{
		m_TimeSinceAttached = 0.f;
	}
	void Attached::Execute(Fedora* owner)
	{
		m_TimeSinceAttached += Game.DeltaTime() * 1.75f;
		// Lerp to target
		if (owner->ObjectTransform.Parent == nullptr)
		{
			float snapRange = 0.08f;
			Vector3 wearingOffset(0, 1.92f, 0);
			Vector3 target = owner->GetOwner()->ObjectTransform.GetMatrix() 
				* Vector4(wearingOffset.x, wearingOffset.y, wearingOffset.z, 1.f);
			float targetYaw = owner->GetOwner()->ObjectTransform.GetYaw();
			float targetPitch = owner->GetOwner()->ObjectTransform.GetPitch();
			float targetRoll = owner->GetOwner()->ObjectTransform.GetRoll();
			owner->ObjectTransform.Position = glm::lerp(owner->ObjectTransform.Position, target, 0.5f * m_TimeSinceAttached);
			owner->ObjectTransform.Position.y = glm::lerp(owner->ObjectTransform.Position.y, target.y, 0.02f);
			owner->ObjectTransform.SetYaw(LerpAngle(owner->ObjectTransform.GetYaw(), targetYaw, 0.1f));
			owner->ObjectTransform.SetPitch(LerpAngle(owner->ObjectTransform.GetPitch(), targetPitch, 0.1f));
			owner->ObjectTransform.SetRoll(LerpAngle(owner->ObjectTransform.GetRoll(), targetRoll, 0.1f));
			float distToTarget = glm::length(target - owner->ObjectTransform.Position);
			// Snap
			if (distToTarget < snapRange)
			{
				owner->AttachToParent(owner->GetOwner());
				owner->ObjectTransform.Position = wearingOffset;
				owner->ObjectTransform.Rotation = Vector3(0, 0, 0);
			}
		}
	}
	void Attached::Exit(Fedora* owner)
	{
		owner->AttachToParent(nullptr);
	}

	// Flying State, fedora is thrown
	void Flying::Enter(Fedora* owner)
	{

	}
	void Flying::Execute(Fedora* owner)
	{
		owner->Move();
		//owner->GetFuturePosition(0.5f);
	}
	void Flying::Exit(Fedora* owner)
	{
	}


	// Global State, Check if hit Goal
	void GlobalState::Enter(Fedora* owner)
	{

	}
	void GlobalState::Execute(Fedora* owner)
	{
		
	}
	void GlobalState::Exit(Fedora* owner)
	{
	}
}