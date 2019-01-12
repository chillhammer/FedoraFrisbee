#include "FedPCH.h"
#include "InputManager.h"

namespace Fed
{
	InputManager& InputManager::Instance()
	{
		static InputManager input;
		return input;
	}
	

}