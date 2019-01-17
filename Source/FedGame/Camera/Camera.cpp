#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Game/GameManager.h>
#include "Camera.h"

namespace Fed
{
	Camera::Camera()
		: m_Speed(1.f)
	{
	}
	void Camera::Init()
	{
		Input.MouseMoved.AddObserver(this);
		m_PrevMousePosition = Input.GetMousePosition();
	}

	void Camera::Update()
	{
		if (Input.IsKeyDown(KEY_W))
		{
			m_Transform.Position += m_Transform.GetHeading() * m_Speed * Game.DeltaTime();
		}
		if (Input.IsKeyDown(KEY_S))
		{
			m_Transform.Position -= m_Transform.GetHeading() * m_Speed * Game.DeltaTime();
		}
		if (Input.IsKeyDown(KEY_D))
		{
			m_Transform.Position += m_Transform.GetSide() * m_Speed * Game.DeltaTime();
		}
		if (Input.IsKeyDown(KEY_A))
		{
			m_Transform.Position -= m_Transform.GetSide() * m_Speed * Game.DeltaTime();
		}

		// Look Around
		UpdateMouseDelta();
		Vector2 mouseDelta = m_DeltaMousePosition;
		float deltaX = mouseDelta.x;
		float deltaY = mouseDelta.y;
		m_Transform.SetPitch(m_Transform.GetPitch() + deltaY * Game.DeltaTime());
		m_Transform.SetYaw(m_Transform.GetYaw() + deltaX * Game.DeltaTime());
		//LOG("DeltaY: {0}", deltaY);

		//LOG("Camera Position: {0}, {1}, {2}", m_Transform.Position.x, m_Transform.Position.y, m_Transform.Position.z);
		//LOG("Camera Heading: {0}, {1}, {2}", m_Transform.GetHeading().x, m_Transform.GetHeading().y, m_Transform.GetHeading().z);
		//LOG("Camera Side: {0}, {1}, {2}", m_Transform.GetSide().x, m_Transform.GetSide().y, m_Transform.GetSide().z);
		//LOG("Camera Up: {0}, {1}, {2}", m_Transform.GetUp().x, m_Transform.GetUp().y, m_Transform.GetUp().z);

	}

	void Camera::OnEvent(const Subject * subject, Event & e)
	{
		Evnt::Dispatch<MouseMovedEvent>(e, EVENT_BIND_FN(Camera, OnMouseMoved));
	}

	Matrix4x4 Camera::GetViewMatrix()
	{
		return glm::lookAt(m_Transform.Position, m_Transform.Position + m_Transform.GetHeading(), m_Transform.GetUp());
		//return m_Transform.GetMatrix();
	}

	// Helper Method
	void Camera::UpdateMouseDelta()
	{
		Vector2 delta = Input.GetMousePosition() - m_PrevMousePosition;
		m_PrevMousePosition = Input.GetMousePosition();
		m_DeltaMousePosition = delta;
	}

	bool Camera::OnMouseMoved(MouseMovedEvent & e)
	{
		//LOG("Camera: Mouse Moved: {0}, {1}", e.GetX(), e.GetY());
		return false;
	}

}