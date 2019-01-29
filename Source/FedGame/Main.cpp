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

using namespace Fed;

int main()
{
	Log::Init();
	Input.Init();

	Game.Init();

	Resources.Init();

	//Loads in Shader
	Shader shader("Shaders/Basic.shader");
	shader.Bind();

	Vector3 lightPosition(1.f, 3.f, 1.f);

	Shader modelShader("Shaders/Model.shader");
	modelShader.Bind();
	modelShader.SetUniform3f("u_LightPosition", lightPosition.x, lightPosition.y, lightPosition.z);

	Texture wood("../Assets/Textures/wood.png");
	wood.LoadTexture();
	BoxMesh boxMesh(wood);
	//ImportedModel suit("../Assets/Models/Nanosuit/nanosuit.obj");
	//ImportedModel robopadron("../Assets/Models/robopadron.obj");
	Model& refModel = SingleMeshModel(boxMesh);
	ModelPtr robopadron = Resources.GetModel("RoboPadron");

	Renderer renderer;

	while (Game.IsRunning())
	{
		// App Render
		renderer.Clear();

		//suit.Draw(modelShader);
		robopadron->Draw(modelShader);

		refModel.Draw(shader);

		shader.Bind();
		shader.SetUniformMat4f("u_MVP", Game.MainCamera.GetProjectionMatrix() * Game.MainCamera.GetViewMatrix());
		
		modelShader.Bind();
		modelShader.SetUniformMat4f("u_MVP", Game.MainCamera.GetProjectionMatrix() * Game.MainCamera.GetViewMatrix());
		

		Game.Run();

	}



	return 0;
}