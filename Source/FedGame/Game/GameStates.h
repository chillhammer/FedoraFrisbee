#pragma once
#include "GameManager.h"
#include <Objects/Agents/FedoraAgent.h>
#include <Objects/Box/WoodenBox.h>
#include <Objects/Fedora/Fedora.h>
namespace Fed
{

	// Simple Test State to Experiment
	STATE_CLASS_SINGLETON(GameManager, Test,	
		Camera m_Camera;
		FedoraAgent m_Agent;
		Vector3 m_Light;
		ShaderPtr m_Shader;
		WoodenBox m_Box;
		Fedora m_Fedora;
	);
}