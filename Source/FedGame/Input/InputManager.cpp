#include "FedPCH.h"
#include "InputManager.h"

namespace Fed
{
	InputManager& InputManager::Instance()
	{
		static InputManager input;
		return input;
	}

	// Allows to force Singleton lazy instantiation 
	void InputManager::Init()
	{
		MouseMoved.AddObserver(this);
		MouseClicked.AddObserver(this);
		MouseReleased.AddObserver(this);
		MouseScrolled.AddObserver(this);
		KeyPressed.AddObserver(this);
		KeyReleased.AddObserver(this);
		m_MousePosition = Vector2(0, 0);
	}

	// Handles Input Changes. Dispatch events to appropriate callback
	void InputManager::OnEvent(const Subject * subject, Event & event)
	{
		Evnt::Dispatch<MouseMovedEvent>(event, EVENT_BIND_FN(InputManager, OnMouseMoved));
		Evnt::Dispatch<MouseButtonPressedEvent>(event, EVENT_BIND_FN(InputManager, OnMouseClicked));
		Evnt::Dispatch<MouseButtonReleasedEvent>(event, EVENT_BIND_FN(InputManager, OnMouseReleased));
		Evnt::Dispatch<MouseScrolledEvent>(event, EVENT_BIND_FN(InputManager, OnMouseScrolled));
		Evnt::Dispatch<KeyPressedEvent>(event, EVENT_BIND_FN(InputManager, OnKeyPressed));
		Evnt::Dispatch<KeyReleasedEvent>(event, EVENT_BIND_FN(InputManager, OnKeyReleased));
	}

	// Getter for Mouse Position
	Vector2 InputManager::GetMousePosition() const
	{
		return m_MousePosition;
	}

	// Only changes local variable, not actual cursor position
	void InputManager::SetMousePosition(Vector2 mousePos)
	{
		m_MousePosition = mousePos;
	}

	bool InputManager::IsKeyDown(int keyCode)
	{
		return m_KeysDown[keyCode];
	}

	#pragma region Callbacks
	bool InputManager::OnMouseMoved(MouseMovedEvent e)
	{
		Vector2 newMousePos(e.GetX(), e.GetY());
		m_MousePosition = newMousePos;
		return false;
	}

	bool InputManager::OnMouseClicked(MouseButtonPressedEvent e)
	{
		return false;
	}

	bool InputManager::OnMouseReleased(MouseButtonReleasedEvent e)
	{
		return false;
	}

	bool InputManager::OnMouseScrolled(MouseScrolledEvent e)
	{
		return false;
	}

	bool InputManager::OnKeyPressed(KeyPressedEvent e)
	{
		int key = e.GetKeyCode();
		m_KeysDown[key] = true;
		return false;
	}

	bool InputManager::OnKeyReleased(KeyReleasedEvent e)
	{
		int key = e.GetKeyCode();
		m_KeysDown[key] = false;
		return false;
	}
	#pragma endregion

}