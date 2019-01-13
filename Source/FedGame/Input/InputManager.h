#pragma once
#include <EventSystem/Events/ApplicationEvent.h>
#include <EventSystem/Events/MouseEvent.h>
#include <EventSystem/Events/KeyEvent.h>
#include <EventSystem/Subject.h>
#include "Keys.h"

#define Input InputManager::Instance()

namespace Fed
{
	class InputManager : IObserver
	{
	public:
		static InputManager& Instance();
		Subject MouseMoved;
		Subject MouseClicked;
		Subject MouseReleased;
		Subject MouseScrolled;
		Subject KeyPressed;
		Subject KeyReleased;

		void Init();
		void OnEvent(const Subject* subject, class Event& event);
		Vector2 GetMousePosition() const;
		bool IsKeyDown(int keyCode);
	private:
		InputManager() {};

		static const int MAX_KEYS = 1024;
		bool m_KeysDown[MAX_KEYS];
		Vector2 m_MousePosition;

		// Callback functions
		bool OnMouseMoved(MouseMovedEvent e);
		bool OnMouseClicked(MouseButtonPressedEvent e);
		bool OnMouseReleased(MouseButtonReleasedEvent e);
		bool OnMouseScrolled(MouseScrolledEvent e);
		bool OnKeyPressed(KeyPressedEvent e);
		bool OnKeyReleased(KeyReleasedEvent e);
	};
}