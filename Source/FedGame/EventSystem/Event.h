#pragma once
#include <FedPCH.h>
#include "EventType.h"
namespace Fed
{
	// An event is the data passed from Subject to Observer
	class Event
	{
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }
		bool Handled = false;

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}


	// Macros for creating new events

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() \
										{ return EventType::##type; }\
									virtual EventType GetEventType() const override \
										{ return GetStaticType(); }\
									virtual const char* GetName() const override \
										{ return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override \
									{ return category; }

	#define EVENT_CLASS_SIMPLE(type, category) \
		class type ## Event : public Event \
		{ \
		public: \
			EVENT_CLASS_TYPE(WindowClose) \
			EVENT_CLASS_CATEGORY(EventCategoryApplication) \
		}

	// Namespace for functions dealing with Event
	namespace Evnt
	{
		template<typename T>
		bool Dispatch(Event& event, std::function<bool(T&)> func)
		{
			if (event.GetEventType() == T::GetStaticType())
			{
				event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	}
}