#include <FedPCH.h>
#include <Resources/ResourceManager.h>
#include "FedoraAgent.h"
#include "Components/FedoraAgentInputPlayer.h"
namespace Fed
{
	FedoraAgent::FedoraAgent()
		: GameObject("RoboPadron")
	{
	}
	FedoraAgent::FedoraAgent(Vector3 position)
		: GameObject("RoboPadron", position)
	{
	}
	FedoraAgent::~FedoraAgent()
	{
		if (m_InputComponent)
			delete m_InputComponent;
	}
	// Heap Allocates a new InputType Component
	void FedoraAgent::SetInputType(AgentInputType inputType)
	{
		if (m_InputType == inputType)
			return;
		if (m_InputType != AgentInputType::NONE)
		{
			delete m_InputComponent;
		}
		m_InputType = inputType;
		switch (inputType)
		{
		case AgentInputType::NONE:
			m_InputComponent = nullptr;
			break;
		case AgentInputType::PLAYER:
			m_InputComponent = new FedoraAgentInputPlayer();
		}
		
	}
	// FedoraAgent keeps reference of camera for player input
	// AI may also use camera reference to decide actions
	void FedoraAgent::SetCameraReference(Camera * camera)
	{
		m_Camera = camera;
	}
	// Updates logic within Fedora Agent
	// Virtually calls logic to either AI or Player Input
	void FedoraAgent::Update()
	{
		if (m_InputComponent)
		{
			m_InputComponent->Update(this);
		}
	}
}
