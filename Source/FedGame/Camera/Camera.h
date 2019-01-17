#pragma once
#include <Transform/Transform.h>
#include <EventSystem/IObserver.h>
#include <EventSystem/Events/MouseEvent.h>

namespace Fed
{
	class Camera : IObserver
	{
	public:
		Camera();
		void Init();
		void Update();
		void OnEvent(const Subject* subject, Event& e);
		Matrix4x4 GetViewMatrix();
	private:
		Transform m_Transform;
		float m_Speed;
		Vector2 m_PrevMousePosition;
		Vector2 m_DeltaMousePosition;
		void UpdateMouseDelta();
	private:
		bool OnMouseMoved(MouseMovedEvent& e);
	};
}