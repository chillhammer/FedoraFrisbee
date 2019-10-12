#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Resources/ResourceManager.h>
#include <Utility/UI/UIElement.h>
#include "GameStates.h"

namespace Fed::GameStates
{
	static void SetupAgent(FedoraAgent& agent, Camera* camera, FrisbeeFieldController* field, Vector3 position, TeamColor teamColor, AgentInputType input = AgentInputType::AI);
	static void UpdateShaders(ShaderPtr shader, ShaderPtr debugShader, ShaderPtr uiShader, const Camera& camera);
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
		m_UIShader = Resources.GetShader("UI");

		for (int i = 0; i < NUM_AGENTS; i++) {
			TeamColor color = (i >= NUM_AGENTS / 2 ? TeamColor::Red : TeamColor::Blue);
			AgentInputType input = (i == 0 ? AgentInputType::PLAYER : AgentInputType::AI);
			SetupAgent(m_Agents[i], &m_Camera, &m_FieldController, Vector3(i, 0, 0), color, input);
		}

		m_Fedora.ObjectTransform.Position = Vector3(0, 0, 0);
		m_Fedora.SetOwner(nullptr);
		m_Fedora.Stop();
		m_Fedora.SetFieldControllerReference(&m_FieldController);
		m_FieldController.SetFedoraReference(&m_Fedora);
		m_FieldController.SetCourtReference(&m_Court);
		m_FieldController.ResetPositions();
	}

	void TeamMatch::Execute(GameManager* owner)
	{
		m_Camera.Update();
		m_FieldController.GetTeam(TeamColor::Blue)->Update();
		m_FieldController.GetTeam(TeamColor::Red)->Update();

		//m_FieldController.GetTeam(TeamColor::Blue)->DebugRenderPositionScores();
		//m_FieldController.GetTeam(TeamColor::Red)->DebugRenderPositionScores();
		for (FedoraAgent& agent : m_Agents)
			agent.Update();
		m_Fedora.Update();

		UpdateShaders(m_Shader, m_DebugShader, m_UIShader, m_Camera);

		

		m_Court.Draw();
		//m_Court.DrawDebugWalls();
		//m_Court.DrawDebugGoals();
		m_Fedora.Draw();
		DrawAgents(m_Agents, NUM_AGENTS);

		UIElement element("Wood");
		element.UITransform.Scale = Vector3(100.0f, 100.0f, 100.0f);
		//element.Draw();

		FontPtr font = Resources.GetFont("Arial");
		font->RenderText("LHTestTestTestTest", Vector2(0.0f, 0.0f), 1.0f, Vector3(1.0f, 1.0f, 1.0f), Game.GetWindow().GetWidth(), Game.GetWindow().GetHeight());
	}

	void TeamMatch::Exit(GameManager* owner)
	{
		m_FieldController.SetFedoraReference(nullptr);
	}

	// Sets up agent initial settings
	static void SetupAgent(FedoraAgent& agent, Camera* camera, FrisbeeFieldController* field, Vector3 position, TeamColor teamColor, AgentInputType input)
	{
		agent.Reset();
		agent.ObjectTransform.Position = position; // Temporary position assignment
		agent.SetInputType(input);
		agent.SetCameraReference(camera);
		agent.SetFieldControllerReference(field);
		agent.SetTeamColor(teamColor);
		field->AddAgentReference(&agent);
	}
	void UpdateShaders(ShaderPtr shader, ShaderPtr debugShader, ShaderPtr uiShader, const Camera& camera)
	{
		shader->Bind();
		shader->SetUniformMat4f("u_ViewProjection", camera.GetProjectionMatrix() * camera.GetViewMatrix());
		debugShader->Bind();
		debugShader->SetUniformMat4f("u_ViewProjection", camera.GetProjectionMatrix() * camera.GetViewMatrix());
		uiShader->Bind();
		uiShader->SetUniformMat4f("u_ViewProjection", camera.GetOrthographicMatrix());

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