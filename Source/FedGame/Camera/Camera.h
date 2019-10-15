#pragma once
#include <Transform/Transform.h>
#include <EventSystem/IObserver.h>

namespace Fed
{
	enum CameraMode { NoClip, Pivot, Frozen };
	class Camera : IObserver
	{
	public:
		Camera();
		void Init();
		void Update();
		void OnEvent(const Subject* subject, Event& e);
		Matrix4x4 GetViewMatrix() const;
		Matrix4x4 GetProjectionMatrix() const;
		static Matrix4x4 GetOrthographicMatrix();
		void SetPivotPosition(Vector3 newPosition);
		void LookInDirection(Vector3 direction);
	public:
		CameraMode Mode;
		Transform ObjectTransform;
	private:
		float m_Speed;
		float m_Sensitivity;
		Vector2 m_PrevMousePosition;
		Vector2 m_DeltaMousePosition;
		float m_Pitch;
		float m_Yaw;
		float m_TargetYaw;
		Vector3 m_PivotPosition;
		Vector3 m_PivotOffset;
		float m_PivotLength;

		Vector3 m_TargetLocation;
	private:
		bool OnMouseMoved(class MouseMovedEvent& e);
		bool OnKeyPressed(class KeyPressedEvent& e);
		void UpdatePivotPosition();
	};
}