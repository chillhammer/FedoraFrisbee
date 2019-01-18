#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Game/GameManager.h>
#include <GLFW/glfw3.h>
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
		//glfwSetInputMode(Game.GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		
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
		float deltaX = m_DeltaMousePosition.x;
		float deltaY = m_DeltaMousePosition.y;

		m_Pitch += deltaY * m_Sensitivity * Game.DeltaTime();
		m_Yaw -= deltaX * m_Sensitivity * Game.DeltaTime();
		m_Pitch = glm::clamp<float>(m_Pitch, -80, 80);
		if (m_Yaw > 360) m_Yaw = 0;
		if (m_Yaw < 0) m_Yaw = 360;

		m_Transform.SetPitch(glm::radians(m_Pitch));
		m_Transform.SetYaw(glm::radians(m_Yaw));
		
		//LOG("Camera Pitch: {0} - Local: {1}", glm::degrees(m_Transform.GetPitch()), m_Pitch);
		LOG("Camera Yaw: {0} - Local: {1}", glm::degrees(m_Transform.GetYaw()), m_Yaw);
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

	static int DELTA_CAP = 100;
	bool Camera::OnMouseMoved(MouseMovedEvent & e)
	{
		//LOG("Camera: Mouse Moved: {0}, {1}", e.GetX(), e.GetY());
		m_DeltaMousePosition = Vector2( e.GetX() - DELTA_CAP, e.GetY() - DELTA_CAP);
		glfwSetCursorPos(Game.GetWindowPtr(), DELTA_CAP, DELTA_CAP);
		return false;
	}

}