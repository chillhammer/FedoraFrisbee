#include <FedPCH.h>
#include <Input/InputManager.h>
#include <EventSystem/Events/MouseEvent.h>
#include <EventSystem/Events/KeyEvent.h>
#include <Game/GameManager.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

namespace Fed
{
	Camera::Camera()
		:	m_Speed(3.8f), m_Sensitivity(40.f), m_Yaw(180), 
			Mode(CameraMode::Pivot), m_PivotLength(4.9f), m_PivotOffset(0, 2.3f, 0.4f),
			m_PivotPosition(0, 0, 0)
	{
		ObjectTransform.Position.z = 2.f;
	}
	void Camera::Init()
	{
		LOG("Initialized Camera");
		Input.MouseMoved.AddObserver(this);
		Input.KeyPressed.AddObserver(this);
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
				ObjectTransform.Position += ObjectTransform.GetHeading() * m_Speed * Game.DeltaTimeUnscaled();
			}
			if (Input.IsKeyDown(KEY_S))
			{
				ObjectTransform.Position -= ObjectTransform.GetHeading() * m_Speed * Game.DeltaTimeUnscaled();
			}
			if (Input.IsKeyDown(KEY_D))
			{
				ObjectTransform.Position += ObjectTransform.GetSide() * m_Speed * Game.DeltaTimeUnscaled();
			}
			if (Input.IsKeyDown(KEY_A))
			{
				ObjectTransform.Position -= ObjectTransform.GetSide() * m_Speed * Game.DeltaTimeUnscaled();
			}
			if (Input.IsKeyDown(KEY_LEFT_SHIFT))
			{
				ObjectTransform.Position -= Vector3(0.f, 2.f, 0.f) * m_Speed * Game.DeltaTimeUnscaled();
			}
			if (Input.IsKeyDown(KEY_SPACE))
			{
				ObjectTransform.Position += Vector3(0.f, 2.f, 0.f) * m_Speed * Game.DeltaTimeUnscaled();
			}

			// Look Around
			m_Pitch -= m_DeltaMousePosition.y * m_Sensitivity * Game.DeltaTimeUnscaled();
			m_Yaw += m_DeltaMousePosition.x * m_Sensitivity * Game.DeltaTimeUnscaled();
			m_Pitch = glm::clamp<float>(m_Pitch, -89, 89);

			ObjectTransform.SetPitch(m_Pitch);
			ObjectTransform.SetYaw(m_Yaw);
		}
		// Third-Person Pivoting
		if (Mode == CameraMode::Pivot)
		{
			// Look Around
			m_Pitch -= m_DeltaMousePosition.y * m_Sensitivity * Game.DeltaTime();
			m_Yaw += m_DeltaMousePosition.x * m_Sensitivity * Game.DeltaTime();
			m_Pitch = glm::clamp<float>(m_Pitch, -10, -10);

			ObjectTransform.SetPitch(m_Pitch);
			ObjectTransform.SetYaw(m_Yaw);

			UpdatePivotPosition();
		}


		// Reset Delta Movement
		m_DeltaMousePosition = Vector2(0, 0);

		// Handling Visible Cursor
		if (Mode == CameraMode::Frozen || (Game.IsPaused() && Mode != CameraMode::NoClip)) {
			Game.GetWindow().SetCursorEnabled(true);
		}
		else {
			Game.GetWindow().SetCursorEnabled(false);
		}

	}

	void Camera::OnEvent(const Subject * subject, Event & e)
	{
		Evnt::Dispatch<MouseMovedEvent>(e, EVENT_BIND_FN(Camera, OnMouseMoved));
		Evnt::Dispatch<KeyPressedEvent>(e, EVENT_BIND_FN(Camera, OnKeyPressed));
		
	}

	Matrix4x4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(ObjectTransform.Position, ObjectTransform.Position + ObjectTransform.GetHeading(), ObjectTransform.GetUp());
	}

	Matrix4x4 Camera::GetProjectionMatrix() const
	{
		float fov = 45;
		float aspect = 0.1f;
		if (Game.GetWindow().GetHeight() != 0 && Game.GetWindow().GetHeight() != 0)
			aspect = (float)Game.GetWindow().GetWidth() / (float)Game.GetWindow().GetHeight();

		return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.f);
	}

	Matrix4x4 Camera::GetOrthographicMatrix() const
	{
		return glm::ortho(0.0f, (float)Game.GetWindow().GetWidth(), (float)Game.GetWindow().GetHeight(), 0.0f);
	}

	// Used to update position when pivoting
	void Camera::SetPivotPosition(Vector3 newPosition)
	{
		m_PivotPosition = newPosition;
		UpdatePivotPosition();
	}
	void Camera::LookInDirection(Vector3 direction)
	{
		direction = glm::normalize(direction);
		float newYaw = glm::degrees(std::atan2(direction.z, direction.x)) + 90;
		float newPitch = glm::degrees(std::atan2(direction.y, direction.x));
		m_Yaw = newYaw;
		m_Pitch = newPitch;
	}
	// Calculate new position when pivoting
	void Camera::UpdatePivotPosition()
	{
		if (Mode != CameraMode::Pivot)
			return;
		Vector3 pivotStick = m_PivotLength * -1 * ObjectTransform.GetHeading();
		Matrix4x4 rotMat = glm::eulerAngleXYZ(0.f, glm::radians(-ObjectTransform.GetYaw()), 0.f);
		Vector3 rotatedPivotOffset = rotMat * Vector4(m_PivotOffset, 1);
		ObjectTransform.Position = m_PivotPosition + rotatedPivotOffset + pivotStick;
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
		if (!Game.IsPaused() || Mode == CameraMode::NoClip)
			Game.GetWindow().SetCursorPosition(DELTA_CAP, DELTA_CAP);
		return false;
	}

	bool Camera::OnKeyPressed(KeyPressedEvent & e)
	{
		switch (e.GetKeyCode())
		{
		case KEY_C:
			Mode = (Mode == CameraMode::NoClip ? CameraMode::Pivot : CameraMode::NoClip);
			break;
		case KEY_V:
			Mode = CameraMode::Frozen;
			break;
		}
		return false;
	}

}