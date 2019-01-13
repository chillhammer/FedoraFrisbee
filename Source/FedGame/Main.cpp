#include <FedPCH.h>
#include <Window/Window.h>
#include <Input/InputManager.h>
#include <Game/Game.h>
#include <GL/glew.h>
#include <glm.hpp>
#include <Graphics/Renderer.h>
#include <Graphics/IndexBuffer.h>
#include <Graphics/Shader.h>
#include <Graphics/VertexBuffer.h>
#include <Graphics/VertexArray.h>
#include <Graphics/Texture.h>
#include <gtc/matrix_transform.hpp>

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

	float positions[] = {
		-0.5f, -0.5f, 0.0f, 0.0f,	//Bottom Left
		 0.5f, -0.5f, 1.0f, 0.0f,	//Bottom Right
		 0.5f,  0.5f, 1.0f, 1.0f,	//Top Right
		-0.5f,  0.5f, 0.0f, 1.0f	//Top Left
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Fed::VertexArray va;
	Fed::VertexBuffer vb(positions, 4 * 4 * sizeof(float));
	Fed::VertexBufferLayout layout;
	layout.Push<float>(2); //Color
	layout.Push<float>(2); //Texture Coords
	va.AddBuffer(vb, layout);
	Fed::IndexBuffer ibo(indices, 6);

	glm::mat4 proj = glm::ortho(-2.f, 2.f, -1.5f, 1.5f, -1.f, 1.f);


	//Loads in Shader
	Fed::Shader shader("../Resources/Shaders/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.6, 0.2, 0.2, 1.0);
	shader.SetUniformMat4f("u_MVP", proj);

	Fed::Texture texture("../Resources/Textures/golemn.png");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	//Sets up Renderer
	Fed::Renderer renderer;

	while (Running)
	{
		// App Render
		glClearColor((Fed::Input.IsKeyDown(KEY_W) ? 1 : 0), 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.Draw(va, ibo, shader);

		shader.Bind();
		shader.SetUniformMat4f("u_MVP", proj);

		game.Run();

		// Poll Window Events
		window.OnUpdate();
	}



	return 0;
}