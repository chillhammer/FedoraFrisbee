#pragma once

namespace Fed
{
	// Interface to recieve events
	class IObserver
	{
		friend class Subject;
	public:
		virtual void OnEvent(Subject* const subject, class Event* const event) = 0;
	};
}