#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Game/GameManager.h>
#include "Camera.h"

namespace Fed
{
	Camera::Camera()
		: m_Speed(1.2f), m_Sensitivity(40.f)
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

		m_Pitch += deltaY * m_Sensitivity * Game.DeltaTime();
		m_Yaw -= deltaX * m_Sensitivity * Game.DeltaTime();
		m_Pitch = glm::clamp<float>(m_Pitch, -80, 80);

		m_Transform.SetPitch(glm::radians(m_Pitch));
		m_Transform.SetYaw(glm::radians(m_Yaw));
		
		LOG("Camera Pitch: {0} - Local: {1}", glm::degrees(m_Transform.GetPitch()), m_Pitch);
		//LOG("Camera Yaw: {0} - Local: {1}", glm::degrees(m_Transform.GetYaw()), m_Yaw);
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
		const int DELTA_CAP = 100;
		Vector2 delta = Input.GetMousePosition() - m_PrevMousePosition;
		m_PrevMousePosition = Input.GetMousePosition();
		if (glm::length2(delta) < DELTA_CAP * DELTA_CAP)
			m_DeltaMousePosition = delta;
		else
		{
			m_DeltaMousePosition = Vector2(0, 0);
			LOG("DELTA CAP! {0}", glm::length(delta));
		}
	}

	bool Camera::OnMouseMoved(MouseMovedEvent & e)
	{
		//LOG("Camera: Mouse Moved: {0}, {1}", e.GetX(), e.GetY());
		return false;
	}

}