#pragma once

#include <EventSystem/Event.h>


namespace Fed
{

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	EVENT_CLASS_SIMPLE(WindowClose, EventCategoryApplication);
	EVENT_CLASS_SIMPLE(AppTick, EventCategoryApplication);
	EVENT_CLASS_SIMPLE(AppUpdate, EventCategoryApplication);
	EVENT_CLASS_SIMPLE(AppRender, EventCategoryApplication);
}