#include <FedPCH.h>
#include <Resources/ResourceManager.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include "FedoraAgent.h"
#include "Components/FedoraAgentInputPlayer.h"
namespace Fed
{
	FedoraAgent::FedoraAgent()
		: GameObject("FedoraAgent"), m_FieldController(nullptr)
	{
		SetBoundingBox(Vector3(0, 1, 0.5f), Vector3(0.5, 1, 0.5));
	}
	FedoraAgent::FedoraAgent(Vector3 position)
		: GameObject("FedoraAgent", position), m_FieldController(nullptr)
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

		// Instatiates New Component
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

	// Access field controller to query information or trigger events
	void FedoraAgent::SetFieldControllerReference(FrisbeeFieldController * controller)
	{
		ASSERT(!m_FieldController, "Can only set Field Controller once, for now.");
		m_FieldController = controller;
		controller->FrisbeeThrown.AddObserver(this);
	}
	FrisbeeFieldController * FedoraAgent::GetFieldController() const
	{
		return m_FieldController;
	}
	// Has Fedora Accessors
	bool FedoraAgent::GetHasFedora() const
	{
		return m_HasFedora;
	}
	void FedoraAgent::SetHasFedora(bool hasFedora)
	{
		m_HasFedora = hasFedora;
	}

	// Handles Events about the match for this agent
	void FedoraAgent::OnEvent(const Subject * subject, Event & event)
	{
		// TODO: Handle Frisbee Thrown. Create OnFrisbeeThrown()
		// Evnt::Dispatch<FrisbeeThrownEvent>(event, EVENT_BIND_FN(FedoraAgent, OnFrisbeeThrown));
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
