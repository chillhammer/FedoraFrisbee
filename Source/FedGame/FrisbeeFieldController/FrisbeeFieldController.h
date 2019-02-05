#pragma once
#include <EventSystem/Events/FrisbeeFieldEvent.h>
#include <EventSystem/Subject.h>
namespace Fed
{
	/**
		Controls the field and logic of a Fedora Frisbee match.
		Handles events between players, and also with frisbee
	**/
	class FrisbeeFieldController
	{
	public:
		FrisbeeFieldController();

	public:
		Subject FrisbeeThrown;
	};
}