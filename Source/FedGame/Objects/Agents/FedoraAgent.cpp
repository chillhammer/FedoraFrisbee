#include <FedPCH.h>
#include <Resources/ResourceManager.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include <Game/GameManager.h>
#include <EventSystem/Events/FrisbeeFieldEvent.h>
#include "FedoraAgent.h"
#include "Components/FedoraAgentInputPlayer.h"
#include "Components/FedoraAgentInputAI.h"
namespace Fed
{
	FedoraAgent::FedoraAgent()
		: GameObject("FedoraAgent"), m_FieldController(nullptr), m_CanGrabTimer(0.f), m_MaxSpeed(10.75f)
	{
		SetBoundingBox(Vector3(0, 1, 0.5f), Vector3(0.5, 1, 0.5));
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
			break;
		case AgentInputType::AI:
			m_InputComponent = new FedoraAgentInputAI();
			break;
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
		controller->FrisbeePickup.AddObserver(this);
	}
	void FedoraAgent::SetTeamReference(Team * team)
	{
		m_Team = team;
	}
	FrisbeeFieldController * FedoraAgent::GetFieldController() const
	{
		return m_FieldController;
	}
	// Has Fedora Accessors
	bool FedoraAgent::GetHasFedora() const
	{
		ASSERT(m_FieldController != nullptr, "Must have field controller reference");
		return m_FieldController->AgentHasFedora(this);
	}

	float FedoraAgent::GetMaxSpeed() const
	{
		return m_MaxSpeed;
	}

	// Returns whether fedora is travelling towards this agent
	bool FedoraAgent::InFedoraPath() const
	{
		ASSERT(m_FieldController != nullptr, "No field controller reference");
		return m_FieldController->IsAgentInFedoraPath(this);
	}

	// Returns whether agent is player controlled
	bool FedoraAgent::IsPlayerControlled() const
	{
		return m_InputType == AgentInputType::PLAYER;
	}

	const Vector3 FedoraAgent::GetFuturePosition(float time) const
	{
		Vector3 futurePos = ObjectTransform.Position + m_Speed * m_Direction * time;
		return futurePos;
	}

	// Updates logic within Fedora Agent
	// Virtually calls logic to either AI or Player Input
	void FedoraAgent::Update()
	{
		if (m_InputComponent)
		{
			m_InputComponent->Update(this);
		}

		// Grab Fedora
		//if (m_CanGrabTimer <= Game.DeltaTime() && m_CanGrabTimer != 0.f)
			//LOG("Can grab now!");
		m_CanGrabTimer = glm::max(0.f, m_CanGrabTimer - Game.DeltaTime());
		if (m_FieldController != nullptr && m_FieldController->IsFedoraFree() && m_CanGrabTimer <= 0.f)
		{
			if (m_FieldController->IsAgentCollidingFedora(this))
			{
				// Send Frisbee Pickup Event
				FrisbeePickupEvent event(m_FieldController->GetFedoraPosition(), *this);
				m_FieldController->FrisbeePickup.Notify(event);
			}
		}

		// Agent collision
		if (m_FieldController != nullptr)
		{
			const FedoraAgent* other = m_FieldController->FindAgentCollidingAgent(this);
			if (other)
			{
				ObjectTransform.Position = m_PrevPosition;
				Vector3 slidingDir = m_BoundingBox.GetSlidingDirection(ObjectTransform, other->ObjectTransform, other->m_BoundingBox, m_Direction);
				ObjectTransform.Position += slidingDir * m_Speed * Game.DeltaTime();
				
				//Steal Fedora
				if (other->GetHasFedora())
				{
					// Send Frisbee Pickup Event
					FrisbeePickupEvent event(m_FieldController->GetFedoraPosition(), *this);
					m_FieldController->FrisbeePickup.Notify(event);
				}
			}
		}
	}
	#pragma region Event Handling
	// Handles Events about the match for this agent
	void FedoraAgent::OnEvent(const Subject * subject, Event & event)
	{
		Evnt::Dispatch<FrisbeeThrownEvent>(event, EVENT_BIND_FN(FedoraAgent, OnFrisbeeThrown));
		Evnt::Dispatch<FrisbeePickupEvent>(event, EVENT_BIND_FN(FedoraAgent, OnFrisbeePickup));
	}
	// Reacts to when frisbee is thrown
	bool FedoraAgent::OnFrisbeeThrown(FrisbeeThrownEvent & e)
	{
		if (e.GetAgent().GetID() == GetID())
		{
			m_CanGrabTimer = 0.5f;
		}
		return false;
	}
	// Reacts to when frisbee is picked up
	bool FedoraAgent::OnFrisbeePickup(FrisbeePickupEvent & e)
	{
		if (e.GetAgent().GetID() == GetID())
		{
			LOG("Fedora Picked up");
			m_CanGrabTimer = 0.1f;
		}
		return false;
	}
	#pragma endregion
}
