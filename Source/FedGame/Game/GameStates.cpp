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

		m_DebugShader = Resources.GetShader("Debug");

		m_Box.ObjectTransform.Scale = Vector3(15, 0.001f, 30);
		m_Agent.ObjectTransform.Position = Vector3(0, 0, 0);
		m_Agent.SetInputType(AgentInputType::PLAYER);
		m_Agent.SetCameraReference(&m_Camera);
		m_Agent2.ObjectTransform.Position = Vector3(0, 0, -10.f);
		m_Agent2.SetInputType(AgentInputType::AI);
		m_Agent2.SetCameraReference(&m_Camera);
		if (!m_Agent.GetFieldController())
		{
			m_Agent.SetFieldControllerReference(&m_FieldController);
			m_Agent2.SetFieldControllerReference(&m_FieldController);
		}

		m_Fedora.SetOwner(&m_Agent);
		m_FieldController.SetFedoraReference(&m_Fedora);
		m_FieldController.AddAgentReference(&m_Agent);
		m_FieldController.AddAgentReference(&m_Agent2);
	}
	void Test::Execute(GameManager* owner)
	{
		m_Camera.Update();
		m_Agent.Update();
		m_Agent2.Update();
		m_Fedora.Update();

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());
		m_DebugShader->Bind();
		m_DebugShader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());

		m_Box.Draw();
		m_Agent.Draw();
		m_Agent2.Draw();
		//m_Agent.DrawBoundingBox();
		m_Fedora.Draw();
		//m_Fedora.DrawBoundingBox();

	}
	void Test::Exit(GameManager* owner)
	{
		m_FieldController.SetFedoraReference(nullptr);
	}
}