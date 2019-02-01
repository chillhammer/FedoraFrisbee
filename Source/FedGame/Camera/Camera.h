#pragma once
#include <Transform/Transform.h>
#include <EventSystem/IObserver.h>
#include <EventSystem/Events/MouseEvent.h>

namespace Fed
{
	enum CameraMode { NoClip, Pivot };
	class Camera : IObserver
	{
	public:
		Camera();
		void Init();
		void Update();
		void OnEvent(const Subject* subject, Event& e);
		Matrix4x4 GetViewMatrix();
		Matrix4x4 GetProjectionMatrix();
		void SetPivotPosition(Vector3 newPosition);
	public:
		CameraMode Mode;
	private:
		Transform m_Transform;
		float m_Speed;
		float m_Sensitivity;
		Vector2 m_PrevMousePosition;
		Vector2 m_DeltaMousePosition;
		float m_Pitch;
		float m_Yaw;
		Vector3 m_PivotPosition;
		Vector3 m_PivotOffset;
		float m_PivotLength;
	private:
		bool OnMouseMoved(MouseMovedEvent& e);
	};
}