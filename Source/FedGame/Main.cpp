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

using namespace Fed;
// Decided on global variable rather than class
bool Running = true;

int main()
{
	Log::Init();
	LOG("Initialized Logger");
	Input.Init();

	Game.Init();

	glm::mat4 proj = glm::ortho(-2.f, 2.f, -1.5f, 1.5f, -1.f, 1.f);
	proj = glm::perspective(glm::radians(45.f), 640.f / 480.f, 0.1f, 100.f);


	//Loads in Shader
	Shader shader("Shaders/Basic.shader");
	shader.Bind();
	//shader.SetUniform3f("u_Color", 0.6, 0.2, 0.2);

	Shader modelShader("Shaders/Model.shader");
	modelShader.Bind();
	modelShader.SetUniform3f("u_Color", 0.4, 0.6, 0.2);

	Texture wood("../Assets/Textures/wood.png");
	wood.LoadTexture();
	BoxMesh boxMesh(wood);
	//Model suit("../Assets/Models/Nanosuit/nanosuit.obj");
	ImportedModel robopadron("../Assets/Models/robopadron.obj");
	Model& refModel = SingleMeshModel(boxMesh);

	Vector3 lightPosition(1.f, 3.f, 1.f);

	//Sets up Renderer
	Renderer renderer;

	while (Running)
	{
		// App Render
		GLCall(glClearColor((Input.IsKeyDown(KEY_W) ? 1 : 0), 0, 1, 1));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		//boxMesh.Draw(shader);
		//suit.Draw(modelShader);
		robopadron.Draw(modelShader);
		refModel.Draw(shader);

		shader.Bind();
		shader.SetUniformMat4f("u_MVP", proj * Game.MainCamera.GetViewMatrix());
		
		modelShader.Bind();
		modelShader.SetUniformMat4f("u_MVP", proj * Game.MainCamera.GetViewMatrix());
		modelShader.SetUniform3f("u_LightPosition", lightPosition.x, lightPosition.y, lightPosition.z);

		Game.Run();

	}



	return 0;
}