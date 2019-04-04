#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Resources/ResourceManager.h>
#include "GameStates.h"

namespace Fed::GameStates
{
	static void SetupAgent(FedoraAgent& agent, Camera* camera, FrisbeeFieldController* field, Vector3 position, TeamColor teamColor, AgentInputType input = AgentInputType::AI);
	static void UpdateShaders(ShaderPtr shader, ShaderPtr debugShader, const Camera& camera);
	static void DrawAgents(FedoraAgent* agents, int size);
	// Team Match State
	void TeamMatch::Enter(GameManager* owner)
	{

		LOG("Entering TeamMatch State");
		m_Camera.Init();
		m_Light = Vector3(0.f, 10.f, -5.f);

		m_Shader = Resources.GetShader("Model");
		m_Shader->Bind();
		m_Shader->SetUniform3f("u_LightPosition", m_Light.x, m_Light.y, m_Light.z);

		m_DebugShader = Resources.GetShader("Debug");

		SetupAgent(m_Agents[0], &m_Camera, &m_FieldController, Vector3(0, 0, 0), TeamColor::Blue, AgentInputType::PLAYER);
		SetupAgent(m_Agents[1], &m_Camera, &m_FieldController, Vector3(0, 0, -10.f), TeamColor::Red);
		SetupAgent(m_Agents[2], &m_Camera, &m_FieldController, Vector3(10.f, 0, -10.f), TeamColor::Blue);

		m_Fedora.ObjectTransform.Position = Vector3(0, 0, 0);
		m_Fedora.SetOwner(nullptr);
		m_Fedora.Stop();
		m_FieldController.SetFedoraReference(&m_Fedora);
		m_FieldController.SetCourtReference(&m_Court);
	}

	void TeamMatch::Execute(GameManager* owner)
	{
		m_Camera.Update();
		for (FedoraAgent& agent : m_Agents)
			agent.Update();
		m_Fedora.Update();

		UpdateShaders(m_Shader, m_DebugShader, m_Camera);

		m_Court.Draw();
		m_Court.DrawDebugWalls();
		m_Fedora.Draw();
		DrawAgents(m_Agents, NUM_AGENTS);
	}

	void TeamMatch::Exit(GameManager* owner)
	{
		m_FieldController.SetFedoraReference(nullptr);
	}

	// Sets up agent initial settings
	static void SetupAgent(FedoraAgent& agent, Camera* camera, FrisbeeFieldController* field, Vector3 position, TeamColor teamColor, AgentInputType input)
	{
		agent.Reset();
		agent.ObjectTransform.Position = position;
		agent.SetInputType(input);
		agent.SetCameraReference(camera);
		agent.SetFieldControllerReference(field);
		agent.SetTeamColor(teamColor);
		field->AddAgentReference(&agent);
	}
	void UpdateShaders(ShaderPtr shader, ShaderPtr debugShader, const Camera& camera)
	{
		shader->Bind();
		shader->SetUniformMat4f("u_ViewProjection", camera.GetProjectionMatrix() * camera.GetViewMatrix());
		debugShader->Bind();
		debugShader->SetUniformMat4f("u_ViewProjection", camera.GetProjectionMatrix() * camera.GetViewMatrix());
	}
	void DrawAgents(FedoraAgent* agents, int size)
	{
		for (int i = 0; i < size; i++)
		{
			FedoraAgent& agent = agents[i];
			agent.Draw();
			agent.DrawSuit();
		}
	}
}