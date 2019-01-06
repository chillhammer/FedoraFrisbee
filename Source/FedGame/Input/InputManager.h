#pragma once
#include <EventSystem/Events/ApplicationEvent.h>
#include <EventSystem/Subject.h>

namespace Fed
{
	class InputManager
	{
	public:
		const Subject MouseMoved;
		const Subject MouseClicked;
	};
}