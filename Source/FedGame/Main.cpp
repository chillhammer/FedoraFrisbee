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

	Vector3 lightPosition(1.f, 3.f, 1.f);

	ShaderPtr shader = Resources.GetShader("Model");
	shader->Bind();
	shader->SetUniform3f("u_LightPosition", lightPosition.x, lightPosition.y, lightPosition.z);

	ModelPtr robopadron = Resources.GetModel("RoboPadron");
	ModelPtr suit = Resources.GetModel("Suit");

	Renderer renderer;

	while (Game.IsRunning())
	{
		// App Render
		renderer.Clear();

		suit->Draw(*shader);
		robopadron->Draw(*shader);

		shader->SetUniformMat4f("u_MVP", Game.MainCamera.GetProjectionMatrix() * Game.MainCamera.GetViewMatrix());

		Game.Run();

	}



	return 0;
}