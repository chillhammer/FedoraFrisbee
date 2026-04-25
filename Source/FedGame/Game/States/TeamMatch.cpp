#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Resources/ResourceManager.h>
#include <Utility/UI/UIElement.h>
#include <Objects/Box/BoxMesh.h>
#include <Audio/AudioEngine.h>
#include <Profiler/FrameProfiler.h>
#include "GameStates.h"

namespace Fed::GameStates
{
	static const std::string CONTROLS_TEXT = "WASD + Mouse to Move and Pass";
	static const std::string GOAL_TEXT = "Score 5 goals to win!";

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
		FontPtr font = Resources.GetFont("InGame");

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
		m_FieldController.SetGameEnded(false);
		m_ScoredTimer = -1.0f;
		m_ControlsTimer = 2.0f;
		m_CachedBlueScore = -1;
		m_CachedRedScore = -1;
		m_RedScoreTextSize = Vector2(0.0f, 0.0f);
		m_ControlsTextSize = font->GetTextSize(CONTROLS_TEXT, 1.0f);
		m_GoalTextSize = font->GetTextSize(GOAL_TEXT, 1.0f);
		m_ControlsBackdropHeight = m_GoalTextSize.y + m_ControlsTextSize.y + 100.0f;

		if (!m_BlueScoreBoard)
			m_BlueScoreBoard = std::make_unique<UIElement>("BlueScoreboard");
		if (!m_RedScoreBoard)
			m_RedScoreBoard = std::make_unique<UIElement>("RedScoreboard");
		if (!m_ControlsBackdrop)
			m_ControlsBackdrop = std::make_unique<UIElement>(Vector4(0.0f, 0.0f, 0.0f, 0.85f));
		UpdateScoreText(font);

		m_PauseScreen.SetFieldControllerReference(&m_FieldController);
		m_EndGameScreen.SetFieldControllerReference(&m_FieldController);
		Audio.SetCameraReference(&m_Camera);
	}

	void TeamMatch::Execute(GameManager* owner)
	{
		{
			FrameProfileScope updateScope("Team.Update");
			m_Camera.Update();
			m_FieldController.GetTeam(TeamColor::Blue)->Update();
			m_FieldController.GetTeam(TeamColor::Red)->Update();

			// Debug Position Scores
			//m_FieldController.GetTeam(TeamColor::Blue)->DebugRenderPositionScores();
			//m_FieldController.GetTeam(TeamColor::Red)->DebugRenderPositionScores();

			for (FedoraAgent& agent : m_Agents)
				agent.Update();
			m_Fedora.Update();
		}

		{
			FrameProfileScope drawWorldScope("Team.DrawWorld");
			UpdateShaders(m_Shader, m_DebugShader, m_SkyboxShader, m_UIShader, m_Camera);

		
			m_Court.Draw();
			m_Fedora.Draw();
			DrawAgents(m_Agents, NUM_AGENTS);


#ifndef __EMSCRIPTEN__
			// Skybox is cut from the web demo (saves ~6 MB of cubemap textures and
			// avoids a WebGL2 cubemap-mip path that's unverified for this codebase).
			m_Skybox.Draw();
#endif
		}

		{
			FrameProfileScope uiScope("Team.UI");
			if (!m_EndGameScreen.IsEnabled())
				m_PauseScreen.UpdateAndDraw();

			m_EndGameScreen.UpdateAndDraw();
			FontPtr font = Resources.GetFont("InGame");

			#pragma region Controls and Goal
			 if (m_FieldController.GetTeam(TeamColor::Blue)->Score == 0 
				 && m_FieldController.GetTeam(TeamColor::Red)->Score == 0
				 && m_FieldController.GetTeam(TeamColor::Blue)->InStandoff()) 
			 {
				DrawControlsPrompt(font);
			 }
			#pragma endregion

		
			#pragma region Score UI
			DrawScoreUI(font);
			#pragma endregion
		}
		
		{
			FrameProfileScope scoreScope("Team.ScoreReset");
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
		
	}

	void TeamMatch::Exit(GameManager* owner)
	{
		m_FieldController.SetFedoraReference(nullptr);
		m_FieldController.GetTeam(TeamColor::Blue)->Score = 0;
		m_FieldController.GetTeam(TeamColor::Red)->Score = 0;
	}

	void TeamMatch::DrawControlsPrompt(const FontPtr& font)
	{
		float midY = Game.GetWindow().GetHeight() * 0.5f;
		m_ControlsBackdrop->UITransform.Position = Vector3(0.0f, midY - m_ControlsBackdropHeight * 0.5f, 0.1f);
		m_ControlsBackdrop->UITransform.Scale = Vector3(Game.GetWindow().GetWidth(), m_ControlsBackdropHeight, 0.1f);
		m_ControlsBackdrop->Draw();

		Vector2 pos = Vector2(Game.GetWindow().GetWidth() * 0.5f, midY - m_ControlsBackdropHeight * 0.15f) - m_ControlsTextSize * 0.5f;
		font->RenderText(CONTROLS_TEXT, pos, 1.0f, Vector3(1.0f, 1.0f, 1.0f));

		pos = Vector2(Game.GetWindow().GetWidth() * 0.5f, midY + m_ControlsBackdropHeight * 0.15f) - m_GoalTextSize * 0.5f;
		font->RenderText(GOAL_TEXT, pos, 1.0f, Vector3(1.0f, 1.0f, 1.0f));
	}

	void TeamMatch::DrawScoreUI(const FontPtr& font)
	{
		m_BlueScoreBoard->UITransform.Scale = Vector3(200.0f, 80.0f, 1.0f);
		m_BlueScoreBoard->Draw();

		m_RedScoreBoard->UITransform.Scale = Vector3(200.0f, 80.0f, 1.0f);
		m_RedScoreBoard->UITransform.Position = Vector3(Game.GetWindow().GetWidth() - m_RedScoreBoard->UITransform.Scale.x, 0.0f, 0.0f);
		m_RedScoreBoard->Draw();

		UpdateScoreText(font);
		font->RenderText(m_BlueScoreText, Vector2(30.0f, 20.0f), 1.0f, Vector3(1.0f, 1.0f, 1.0f));
		font->RenderText(m_RedScoreText, Vector2(Game.GetWindow().GetWidth() - 30.0f - m_RedScoreTextSize.x, 20.0f), 1.0f, Vector3(1.0f, 1.0f, 1.0f));
	}

	void TeamMatch::UpdateScoreText(const FontPtr& font)
	{
		int blueScore = m_FieldController.GetTeam(TeamColor::Blue)->Score;
		if (blueScore != m_CachedBlueScore)
		{
			m_CachedBlueScore = blueScore;
			m_BlueScoreText = "Blue: " + std::to_string(blueScore);
		}

		int redScore = m_FieldController.GetTeam(TeamColor::Red)->Score;
		if (redScore != m_CachedRedScore)
		{
			m_CachedRedScore = redScore;
			m_RedScoreText = "Red: " + std::to_string(redScore);
			m_RedScoreTextSize = font->GetTextSize(m_RedScoreText, 1.0f);
		}
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
