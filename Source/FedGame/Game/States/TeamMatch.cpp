#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Resources/ResourceManager.h>
#include <Utility/UI/UIElement.h>
#include <Objects/Box/BoxMesh.h>
#include "GameStates.h"

namespace Fed::GameStates
{
	static void SetupAgent(FedoraAgent& agent, Camera* camera, FrisbeeFieldController* field, Vector3 position, TeamColor teamColor, AgentInputType input = AgentInputType::AI);
	static void UpdateShaders(ShaderPtr shader, ShaderPtr debugShader, ShaderPtr skyboxShader, ShaderPtr uiShader, const Camera& camera);
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
		m_SkyboxShader = Resources.GetShader("Skybox");
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
		m_ScoredTimer = -1.0f;
		m_PauseScreen.SetFieldControllerReference(&m_FieldController);
	}

	void TeamMatch::Execute(GameManager* owner)
	{
		m_Camera.Update();
		m_FieldController.GetTeam(TeamColor::Blue)->Update();
		m_FieldController.GetTeam(TeamColor::Red)->Update();

		// Debug Position Scores
		//m_FieldController.GetTeam(TeamColor::Blue)->DebugRenderPositionScores();
		//m_FieldController.GetTeam(TeamColor::Red)->DebugRenderPositionScores();

		for (FedoraAgent& agent : m_Agents)
			agent.Update();
		m_Fedora.Update();

		UpdateShaders(m_Shader, m_DebugShader, m_SkyboxShader, m_UIShader, m_Camera);

		
		m_Court.Draw();
		m_Fedora.Draw();
		DrawAgents(m_Agents, NUM_AGENTS);


		m_Skybox.Draw();	

		m_PauseScreen.UpdateAndDraw();

		#pragma region Score UI
		UIElement blueScoreBoard(Vector4(0.1f, 0.1f, 0.5f, 1.0f));
		blueScoreBoard.UITransform.Scale = Vector3(200.0f, 80.f, 1.0f);
		blueScoreBoard.Draw();

		UIElement redScoreBoard(Vector4(0.5f, 0.1f, 0.1f, 1.0f));
		redScoreBoard.UITransform.Scale = Vector3(200.0f, 80.f, 1.0f);
		redScoreBoard.UITransform.Position = Vector3(Game.GetWindow().GetWidth() - redScoreBoard.UITransform.Scale.x, 0.0f, 0.0f);
		redScoreBoard.Draw();

		std::stringstream scoreStream;
		scoreStream << "Blue: " << m_FieldController.GetTeam(TeamColor::Blue)->Score
					<< "                                                                        "
					<< "Red: " << m_FieldController.GetTeam(TeamColor::Red)->Score;

		FontPtr font = Resources.GetFont("Arial");
		font->RenderText(scoreStream.str(), Vector2(20.0f, 20.0f), 1.0f, Vector3(1.0f, 1.0f, 1.0f));
		#pragma endregion

		#pragma region Scored Delay before Resetting
		float scoreDelay = 1.5f;
		if (m_FieldController.GetScored()) {
			if (m_ScoredTimer == -1.0f)
				m_ScoredTimer = m_ScoredTimer = scoreDelay;

			m_ScoredTimer = glm::max(0.0f, m_ScoredTimer - Game.DeltaTime());
			if (m_ScoredTimer == 0.0f) {
				m_FieldController.ResetPositions();
				m_ScoredTimer = -1.f;
			}
		}
		#pragma endregion
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
		agent.SetCameraReference(camera);
		agent.SetInputType(input);
		agent.SetFieldControllerReference(field);
		agent.SetTeamColor(teamColor);
		field->AddAgentReference(&agent);
	}
	void UpdateShaders(ShaderPtr shader, ShaderPtr debugShader, ShaderPtr skyboxShader, ShaderPtr uiShader, const Camera& camera)
	{
		Matrix4x4 projView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
		Matrix4x4 skyboxProjView = projView;
		skyboxProjView[3] = { 0.0f, 0.0f, 0.0f, 1.0f };


		shader->Bind();
		shader->SetUniformMat4f("u_ViewProjection", projView);
		skyboxShader->Bind();
		skyboxShader->SetUniformMat4f("u_ViewProjection", skyboxProjView);
		debugShader->Bind();
		debugShader->SetUniformMat4f("u_ViewProjection", projView);


		uiShader->Bind();
		uiShader->SetUniformMat4f("u_Projection", camera.GetOrthographicMatrix());

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