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

		bool IsAgentCollidingFedora(class FedoraAgent* agent);
		bool IsFedoraFree() const;
		void SetFedoraReference(class Fedora* fedora);
	public:
		Subject FrisbeeThrown;

	private:
		class Fedora* m_Fedora;
	};
}