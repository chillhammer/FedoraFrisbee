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

using namespace Fed;
// Decided on global variable rather than class
bool Running = true;

int main()
{
	Log::Init();
	LOG("Initialized Logger");
	Input.Init();

	Game.Init();

	BoxMesh boxMesh;

	glm::mat4 proj = glm::ortho(-2.f, 2.f, -1.5f, 1.5f, -1.f, 1.f);
	proj = glm::perspective(glm::radians(45.f), 640.f / 480.f, 0.f, 1000.f);


	//Loads in Shader
	Shader shader("Shaders/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.6, 0.2, 0.2, 1.0);

	Texture texture("../Assets/Textures/wood.png");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	//Sets up Renderer
	Renderer renderer;

	while (Running)
	{
		// App Render
		GLCall(glClearColor((Input.IsKeyDown(KEY_W) ? 1 : 0), 0, 1, 1));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		Msh::DrawMesh(boxMesh, shader);

		shader.Bind();
		shader.SetUniformMat4f("u_MVP", proj * Game.MainCamera.GetViewMatrix());

		Game.Run();

	}



	return 0;
}