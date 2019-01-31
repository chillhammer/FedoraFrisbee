#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Game/GameManager.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

namespace Fed
{
	Camera::Camera()
		:	m_Speed(3.4f), m_Sensitivity(40.f), m_Yaw(270), 
			Mode(CameraMode::Pivot), m_PivotLength(4.6f), m_PivotPosition(0, 2.3f, 0.4f)
	{
		m_Transform.Position.z = 2.f;
	}
	void Camera::Init()
	{
		LOG("Initialized Camera");
		Input.MouseMoved.AddObserver(this);
		m_PrevMousePosition = Input.GetMousePosition();

		// Invisible Cursor
		Game.GetWindow().SetCursorEnabled(false);
	}

	void Camera::Update()
	{
		// First Person Movement
		if (Mode == CameraMode::NoClip)
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
			if (Input.IsKeyDown(KEY_LEFT_SHIFT))
			{
				m_Transform.Position -= Vector3(0.f, 1.f, 0.f) * m_Speed * Game.DeltaTime();
			}
			if (Input.IsKeyDown(KEY_SPACE))
			{
				m_Transform.Position += Vector3(0.f, 1.f, 0.f) * m_Speed * Game.DeltaTime();
			}

			// Look Around
			m_Pitch -= m_DeltaMousePosition.y * m_Sensitivity * Game.DeltaTime();
			m_Yaw += m_DeltaMousePosition.x * m_Sensitivity * Game.DeltaTime();
			m_Pitch = glm::clamp<float>(m_Pitch, -89, 89);
		}
		// Third-Person Pivoting
		if (Mode == CameraMode::Pivot)
		{
			// Look Around
			m_Pitch -= m_DeltaMousePosition.y * m_Sensitivity * Game.DeltaTime();
			m_Yaw += m_DeltaMousePosition.x * m_Sensitivity * Game.DeltaTime();
			m_Pitch = glm::clamp<float>(m_Pitch, -89, 89);

			m_Transform.SetPitch(m_Pitch);
			m_Transform.SetYaw(m_Yaw);

			Vector3 pivotStick = m_PivotLength * -1 * m_Transform.GetHeading();
			m_Transform.Position = m_PivotPosition + pivotStick;
		}

		m_Transform.SetPitch(m_Pitch);
		m_Transform.SetYaw(m_Yaw);
		// Reset Delta Movement
		m_DeltaMousePosition = Vector2(0, 0);
		
		//LOG("Camera Pitch: {0} - Local: {1}", m_Transform.GetPitch(), m_Pitch);
		//LOG("Camera Yaw: {0} - Local: {1}", m_Transform.GetYaw(), m_Yaw);

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
	}

	Matrix4x4 Camera::GetProjectionMatrix()
	{
		float fov = 45;
		float aspect = (float)Game.GetWindow().GetWidth() / (float) Game.GetWindow().GetHeight();
		return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.f);
	}

	static int DELTA_CAP = 300;
	// Sets delta mouse movement
	bool Camera::OnMouseMoved(MouseMovedEvent & e)
	{
		//LOG("Camera: Mouse Moved: {0}, {1}", e.GetX(), e.GetY());
		if (Input.GetMousePosition() != Vector2(0, 0)) // Initial Mouse Delta Jump Ignored
		{
			m_DeltaMousePosition = Vector2(e.GetX() - DELTA_CAP, e.GetY() - DELTA_CAP);
		}
		else
		{
			LOG("Inital Camera Jump");
		}
		Game.GetWindow().SetCursorPosition(DELTA_CAP, DELTA_CAP);
		return false;
	}

}