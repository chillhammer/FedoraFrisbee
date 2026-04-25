#include <FedPCH.h>
#include <Input/InputManager.h>
#include <EventSystem/Events/MouseEvent.h>
#include <EventSystem/Events/KeyEvent.h>
#include <Game/GameManager.h>
#include <Game/States/GameStates.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

namespace Fed
{
	Camera::Camera()
		:	m_Speed(7.8f),
#ifdef __EMSCRIPTEN__
			m_Sensitivity(4.0f),
#else
			m_Sensitivity(12.f),
#endif
			m_Pitch(-10.0f), m_Yaw(180), m_TargetYaw(180),
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
		m_DeltaMousePosition = Vector2(0.0f, 0.0f);
		m_PrevMousePosition = Input.GetMousePosition();
		m_Pitch = -10.0f;
		m_Yaw = 180.0f;
		m_TargetYaw = m_Yaw;
		m_PivotPosition = Vector3(0.0f, 0.0f, 0.0f);
		ObjectTransform.Position = Vector3(0.0f, 2.3f, 2.0f);
		ObjectTransform.Rotation = Vector3(0.0f, 180.0f, 0.0f);

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

		/*
		// Handling Visible Cursor
		if (Game.IsPaused()) {
			Game.GetWindow().SetCursorEnabled(true);
		}
		else {
			Game.GetWindow().SetCursorEnabled(false);
		}
		*/
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

	Matrix4x4 Camera::GetOrthographicMatrix()
	{
		return glm::ortho(0.0f, (float)Game.GetWindow().GetWidth(), (float)Game.GetWindow().GetHeight(), 0.0f);
	}

	// Used to update position when pivoting
	void Camera::SetPivotPosition(Vector3 newPosition)
	{
		if (!IsFiniteVector(newPosition))
			return;
		m_PivotPosition = newPosition;
		UpdatePivotPosition();
	}
	void Camera::LookInDirection(Vector3 direction)
	{
		direction.y = 0.0f;
		direction = SafeNormalize(direction, Vector3(0.0f, 0.0f, 1.0f));
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
		if (!IsFiniteVector(m_PivotPosition))
			m_PivotPosition = Vector3(0.0f, 0.0f, 0.0f);
		if (!std::isfinite(ObjectTransform.GetYaw()) || !std::isfinite(ObjectTransform.GetPitch()))
		{
			ObjectTransform.SetYaw(180.0f);
			ObjectTransform.SetPitch(-10.0f);
		}
		Vector3 pivotStick = m_PivotLength * -1 * ObjectTransform.GetHeading();
		Matrix4x4 rotMat = glm::eulerAngleXYZ(0.f, glm::radians(-ObjectTransform.GetYaw()), 0.f);
		Vector3 rotatedPivotOffset = rotMat * Vector4(m_PivotOffset, 1);
		ObjectTransform.Position = m_PivotPosition + rotatedPivotOffset + pivotStick;
	}

	static int DELTA_CAP = 300;
	// Sets delta mouse movement
	bool Camera::OnMouseMoved(MouseMovedEvent & e)
	{
		if (Game.GetState() == GameStates::MainMenu::Instance()) return false;
		//LOG("Camera: Mouse Moved: {0}, {1}", e.GetX(), e.GetY());
#ifdef __EMSCRIPTEN__
		Vector2 mousePosition(e.GetX(), e.GetY());
		if (m_PrevMousePosition != Vector2(0, 0) && !Game.IsPaused())
		{
			m_DeltaMousePosition = mousePosition - m_PrevMousePosition;
			if (glm::length2(m_DeltaMousePosition) > DELTA_CAP * DELTA_CAP)
				m_DeltaMousePosition = Vector2(0.0f, 0.0f);
		}
		m_PrevMousePosition = mousePosition;
#else
		float midWindowX = Game.GetWindow().GetWidth() * 0.5f;
		float midWindowY = Game.GetWindow().GetHeight() * 0.5f;
		if (Input.GetMousePosition() != Vector2(0, 0) && !Game.IsPaused()) // Initial Mouse Delta Jump Ignored
		{
			m_DeltaMousePosition = Vector2(e.GetX() - midWindowX, e.GetY() - midWindowY);
			if (glm::length2(m_DeltaMousePosition) > DELTA_CAP * DELTA_CAP)
				m_DeltaMousePosition = Vector2(0.0f, 0.0f);
		}
		if (!Game.IsPaused())
			Game.GetWindow().SetCursorPosition(midWindowX, midWindowY);
#endif
		return false;
	}

	bool Camera::OnKeyPressed(KeyPressedEvent & e)
	{
		if (Game.IsPaused()) return false;
		switch (e.GetKeyCode())
		{
		case KEY_C:
			Mode = (Mode == CameraMode::NoClip ? CameraMode::Pivot : CameraMode::NoClip);
			break;
		case KEY_V:
			if (Mode == CameraMode::Frozen)
				Mode = CameraMode::NoClip;
			else
				Mode = CameraMode::Frozen;
			break;

		case KEY_ESCAPE:
			if (Mode == CameraMode::NoClip && Game.IsPaused())
				Mode = CameraMode::Frozen;
			else if (Mode == CameraMode::Frozen && !Game.IsPaused())
				Mode = CameraMode::NoClip;
			break;
		}
		return false;
	}

}
