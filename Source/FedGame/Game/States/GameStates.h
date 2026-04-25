#pragma once
#include <memory>
#include <string>
#include "../GameManager.h"
#include <Objects/Agents/FedoraAgent.h>
#include <Objects/Box/WoodenBox.h>
#include <Objects/Fedora/Fedora.h>
#include <Objects/Court/Court.h>
#include <Graphics/Model/Skybox.h>
#include <Graphics/Text/Font.h>
#include <UI/Button.h>
#include <Utility/UI/UIElement.h>
#include <Game/Screens/PauseScreen.h>
#include <Game/Screens/EndGameScreen.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
namespace Fed::GameStates
{

	// Simple Test State to Experiment
	STATE_CLASS_SINGLETON(GameManager, Test,	
		Camera m_Camera;
		FedoraAgent m_Agent;
		FedoraAgent m_Agent2;
		FedoraAgent m_Agent3;
		Vector3 m_Light;
		ShaderPtr m_Shader;
		ShaderPtr m_DebugShader;
		WoodenBox m_Box;
		Fedora m_Fedora;
		Court m_Court;
		FrisbeeFieldController m_FieldController;
	);

	

	STATE_CLASS_SINGLETON(GameManager, TeamMatch,
		Camera m_Camera;
		static const int NUM_AGENTS = 6;
		FedoraAgent m_Agents[NUM_AGENTS];
		Vector3 m_Light;
		ShaderPtr m_Shader;
		ShaderPtr m_DebugShader;
		ShaderPtr m_SkyboxShader;
		ShaderPtr m_UIShader;
		Skybox m_Skybox;
		Court m_Court;
		Fedora m_Fedora;
		FrisbeeFieldController m_FieldController;
		float m_ScoredTimer;
		PauseScreen m_PauseScreen;
		EndGameScreen m_EndGameScreen;
		float m_ControlsTimer;
		std::unique_ptr<UIElement> m_BlueScoreBoard;
		std::unique_ptr<UIElement> m_RedScoreBoard;
		std::unique_ptr<UIElement> m_ControlsBackdrop;
		std::string m_BlueScoreText;
		std::string m_RedScoreText;
		Vector2 m_RedScoreTextSize;
		Vector2 m_ControlsTextSize;
		Vector2 m_GoalTextSize;
		float m_ControlsBackdropHeight;
		int m_CachedBlueScore;
		int m_CachedRedScore;
		void DrawControlsPrompt(const FontPtr& font);
		void DrawScoreUI(const FontPtr& font);
		void UpdateScoreText(const FontPtr& font);
	);

	STATE_CLASS_SINGLETON(GameManager, MainMenu,
		ShaderPtr m_UIShader;
		Button m_Start;
	);
}
