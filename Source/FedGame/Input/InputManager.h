#pragma once
#include <EventSystem/Events/ApplicationEvent.h>
#include <EventSystem/Events/MouseEvent.h>
#include <EventSystem/Events/KeyEvent.h>
#include <EventSystem/Subject.h>

#define Input InputManager::Instance()

namespace Fed
{
	class InputManager
	{
	public:
		static InputManager& Instance();
		const Subject MouseMoved;
		const Subject MouseClicked;
		const Subject MouseReleased;
		const Subject MouseScrolled;
		const Subject KeyPressed;
		const Subject KeyReleased;
	private:
		InputManager() {};
	};
}