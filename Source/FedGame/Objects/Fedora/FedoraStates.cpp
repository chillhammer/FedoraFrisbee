#include <FedPCH.h>
#include "FedoraStates.h"
/**
	Each state runs code that determines Fedora, the hat, behavior
**/
namespace Fed::FedoraStates
{
	// Attached State, fedora is worn
	void Attached::Enter(Fedora* owner)
	{
		
	}
	void Attached::Execute(Fedora* owner)
	{

	}
	void Attached::Exit(Fedora* owner)
	{
	}

	// Flying State, fedora is thrown
	void Flying::Enter(Fedora* owner)
	{

	}
	void Flying::Execute(Fedora* owner)
	{
		owner->Move();
	}
	void Flying::Exit(Fedora* owner)
	{
	}
}