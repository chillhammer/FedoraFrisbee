#include <FedPCH.h>
#include <Window/Window.h>
#include <Input/InputManager.h>

// Decided on global variable rather than class
bool Running = true;

int main()
{
	Fed::Log::Init();
	LOG("Initialized Logger");
	Fed::Input.Init();

	Fed::WindowProps props;
	Fed::Window window(props);

	while (Running)
	{
		// App Render
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Poll Window Events
		window.OnUpdate();
	}



	return 0;
}