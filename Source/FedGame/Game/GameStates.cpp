#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Resources/ResourceManager.h>
#include "GameStates.h"
/**
	Each state runs code that determines the course of the game.
	Each state has local variables which can be seen in GameManager.h
**/
namespace Fed
{
	// Test State
	void Test::Enter(GameManager* owner)
	{
		LOG("Entering Test State in Game Object");
		m_Camera.Init();
		m_Light = Vector3(0.f, 10.f, -5.f);

		m_Shader = Resources.GetShader("Model");
		m_Shader->Bind();
		m_Shader->SetUniform3f("u_LightPosition", m_Light.x, m_Light.y, m_Light.z);

		m_Box.ObjectTransform.Scale = Vector3(12, 0.001f, 20);
		m_Agent.SetInputType(AgentInputType::PLAYER);
		m_Agent.SetCameraReference(&m_Camera);

		//m_Fedora.ObjectTransform.Position = Vector3(-2, 1.92f, -2);
		m_Fedora.ObjectTransform.Position = Vector3(0, 1.92f, 0);
		m_Fedora.AttachToParent(&m_Agent);
	}
	void Test::Execute(GameManager* owner)
	{
		m_Camera.Update();
		m_Agent.Update();

		m_Shader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());

		m_Box.Draw();
		m_Agent.Draw();
		m_Fedora.Draw();

	}
	void Test::Exit(GameManager* owner)
	{
	}
}