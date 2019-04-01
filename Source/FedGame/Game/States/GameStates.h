#pragma once
#include "../GameManager.h"
#include <Objects/Agents/FedoraAgent.h>
#include <Objects/Box/WoodenBox.h>
#include <Objects/Fedora/Fedora.h>
#include <Objects/Court/Court.h>
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
		static const int NUM_AGENTS = 3;
		FedoraAgent m_Agents[NUM_AGENTS];
		FedoraAgent m_TestAgent;
		Vector3 m_Light;
		ShaderPtr m_Shader;
		ShaderPtr m_DebugShader;
		Court m_Court;
		Fedora m_Fedora;
		FrisbeeFieldController m_FieldController;
	);
}