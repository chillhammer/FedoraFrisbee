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

using namespace Fed;

int main()
{
	// Setting Up Dependencies
	Log::Init();
	Input.Init();
	Game.Init();
	// Loads All Resources
	Resources.Init();

	Renderer renderer;
	Game.Start();
	while (Game.IsRunning())
	{
		// App Render
		renderer.Clear();

		Game.Run();

	}



	return 0;
}