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
		m_Light = Vector3(1.f, 3.f, 1.f);

		m_Shader = Resources.GetShader("Model");
		m_Shader->Bind();
		m_Shader->SetUniform3f("u_LightPosition", m_Light.x, m_Light.y, m_Light.z);

		m_Box.ObjectTransform.Scale = Vector3(12, 0.001f, 20);
		m_Agent.SetInputType(AgentInputType::PLAYER);
		m_Agent.SetCameraReference(&m_Camera);
	}
	void Test::Execute(GameManager* owner)
	{
		m_Camera.Update();

		m_Agent.Update();
		m_Agent.Draw();
		m_Box.Draw();

		if (Input.IsKeyDown(KEY_J))
		{
			m_Agent.ObjectTransform.Position += Vector3(0.01f, 0, 0);
		}

		m_Shader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());
	}
	void Test::Exit(GameManager* owner)
	{
	}
}