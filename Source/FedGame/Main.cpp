#include <FedPCH.h>
#include <Window/Window.h>
#include <Input/InputManager.h>
#include <Game/GameManager.h>
#include <GL/glew.h>
#include <glm.hpp>
#include <Graphics/OpenGL/Renderer.h>
#include <Graphics/OpenGL/IndexBuffer.h>
#include <Graphics/OpenGL/Shader.h>
#include <Graphics/OpenGL/VertexBuffer.h>
#include <Graphics/OpenGL/VertexArray.h>
#include <Graphics/OpenGL/Texture.h>
#include <Objects/Box/BoxMesh.h>
#include <gtc/matrix_transform.hpp>
#include <Graphics/Model/Model.h>
#include <Graphics/Model/ImportedModel.h>
#include <Graphics/Model/SingleMeshModel.h>
#include <Resources/ResourceManager.h>
#include <Objects/Agents/FedoraAgent.h>
#include <Objects/Box/WoodenBox.h>
#include <EventSystem/Events/KeyEvent.h>
#include <Input/Keys.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace Fed;

#ifdef __EMSCRIPTEN__
extern "C" EMSCRIPTEN_KEEPALIVE void FedoraFrisbeeSendEscape()
{
	KeyPressedEvent event(KEY_ESCAPE, 0);
	Input.KeyPressed.Notify(event);
}
#endif

namespace {
	// One iteration of the game loop. Pulled out so emscripten_set_main_loop_arg
	// can call it from the browser's requestAnimationFrame, while desktop keeps
	// driving it from a plain while().
	void RunFrame(void* userData)
	{
		Renderer* renderer = static_cast<Renderer*>(userData);
		renderer->Clear();
		Game.Run();
	}
}

int main()
{
	// Setting Up Dependencies
	Log::Init();
	Input.Init();
	Game.Init();
	// Loads All Resources
	Resources.Init();

	static Renderer renderer;
	Game.Start();

#ifdef __EMSCRIPTEN__
	// fps=0 → use requestAnimationFrame; simulate_infinite_loop=1 unwinds main()
	// so destructors of stack objects don't fire mid-frame.
	emscripten_set_main_loop_arg(&RunFrame, &renderer, 0, 1);
#else
	while (Game.IsRunning())
	{
		RunFrame(&renderer);
	}
#endif

	return 0;
}
