#include <FedPCH.h>
#include <Window/Window.h>
#include <Input/InputManager.h>
#include <Game/Game.h>

// Decided on global variable rather than class
bool Running = true;

int main()
{
	Fed::Log::Init();
	LOG("Initialized Logger");
	Fed::Input.Init();

	Fed::WindowProps props;
	Fed::Window window(props);
	Fed::Game game;

	while (Running)
	{
		// App Render
		glClearColor((Fed::Input.IsKeyDown(KEY_W) ? 1 : 0), 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game.Run();

		// Poll Window Events
		window.OnUpdate();
	}



	return 0;
}