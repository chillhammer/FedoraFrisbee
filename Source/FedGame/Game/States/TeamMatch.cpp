#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Resources/ResourceManager.h>
#include "GameStates.h"
namespace Fed::GameStates
{
	static void SetupAgent(FedoraAgent& agent, Camera* camera, FrisbeeFieldController* field, Vector3 position, TeamColor teamColor, AgentInputType input = AgentInputType::AI);
	// Team Match State
	void TeamMatch::Enter(GameManager* owner)
	{
		m_Agents.emplace_back();
		m_Agents.emplace_back();
		m_Agents.emplace_back();


		LOG("Entering TeamMatch State");
		m_Camera.Init();
		m_Light = Vector3(0.f, 10.f, -5.f);

		m_Shader = Resources.GetShader("Model");
		m_Shader->Bind();
		m_Shader->SetUniform3f("u_LightPosition", m_Light.x, m_Light.y, m_Light.z);

		m_DebugShader = Resources.GetShader("Debug");

		m_Box.ObjectTransform.Scale = Vector3(15, 0.001f, 30);
		SetupAgent(m_Agents[0], &m_Camera, &m_FieldController, Vector3(0, 0, 0), TeamColor::Blue, AgentInputType::PLAYER);
	}

	void TeamMatch::Execute(GameManager* owner)
	{
		m_Camera.Update();
		m_Agents[0].Update();

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());
		m_DebugShader->Bind();
		m_DebugShader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());

		m_Box.Draw();
		m_Agents[0].Draw();
	}

	void TeamMatch::Exit(GameManager* owner)
	{

	}

	// Sets up agent initial settings
	static void SetupAgent(FedoraAgent& agent, Camera* camera, FrisbeeFieldController* field, Vector3 position, TeamColor teamColor, AgentInputType input = AgentInputType::AI)
	{
		agent.ObjectTransform.Position = position;
		agent.SetInputType(input);
		agent.SetCameraReference(camera);
		agent.SetFieldControllerReference(field);
		agent.SetTeamColor(teamColor);
	}
}