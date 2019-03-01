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
	}

	void TeamMatch::Execute(GameManager* owner)
	{

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