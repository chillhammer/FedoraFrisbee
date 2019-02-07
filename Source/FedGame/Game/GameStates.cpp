#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Resources/ResourceManager.h>
#include "GameStates.h"
/**
	Each state runs code that determines the course of the game.
	Each state has local variables which can be seen in GameManager.h
**/
namespace Fed::GameStates
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
		m_Agent.ObjectTransform.Position = Vector3(0, 0, 0);
		m_Agent.SetInputType(AgentInputType::PLAYER);
		m_Agent.SetCameraReference(&m_Camera);
		if (!m_Agent.GetFieldController())
			m_Agent.SetFieldControllerReference(&m_FieldController);

		m_Fedora.SetOwner(&m_Agent);
		m_FieldController.FrisbeeThrown.AddObserver(&m_Fedora);
	}
	void Test::Execute(GameManager* owner)
	{
		m_Camera.Update();
		m_Agent.Update();
		m_Fedora.Update();

		m_Shader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());

		m_Box.Draw();
		m_Agent.Draw();
		//m_Agent.DrawBoundingBox();
		m_Fedora.Draw();

	}
	void Test::Exit(GameManager* owner)
	{
	}
}