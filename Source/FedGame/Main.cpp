#include <FedPCH.h>
#include <Window/Window.h>
int main()
{
	Fed::Log::Init();
	LOG("Initialized Logger");

	Fed::WindowProps props;
	Fed::Window window(props);

	while (true)
	{
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		window.OnUpdate();
	}

	std::cin.get();



	return 0;
}