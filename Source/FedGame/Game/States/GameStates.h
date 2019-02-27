#pragma once
#include "../GameManager.h"
#include <Objects/Agents/FedoraAgent.h>
#include <Objects/Box/WoodenBox.h>
#include <Objects/Fedora/Fedora.h>
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
		FrisbeeFieldController m_FieldController;
	);

	STATE_CLASS_SINGLETON(GameManager, TeamMatch,

	);
}