#include <FedPCH.h>
#include <Resources/ResourceManager.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include <Game/GameManager.h>
#include <EventSystem/Events/FrisbeeFieldEvent.h>
#include <EventSystem/IObserver.h>
#include "FedoraAgent.h"
#include "Components/FedoraAgentInputPlayer.h"
#include "Components/FedoraAgentInputComponent.h"
#include "Components/FedoraAgentInputAI.h"
namespace Fed
{
	FedoraAgent::FedoraAgent() 
		: GameObject("FedoraAgent"), m_FieldController(nullptr), m_CanGrabTimer(0.f), m_MaxSpeed(10.75f), 
			m_SuitModel(Resources.GetModel("SuitBlue")), m_InputType(AgentInputType::NONE), m_InputComponent(nullptr), m_Team(nullptr),
			INVULNERABLE_TIME(1.0f)
	{
		SetBoundingBox(Vector3(0, 1, 0), Vector3(0.5, 1, 0.5));
	}
	FedoraAgent::FedoraAgent(const FedoraAgent & other)
	{
		ASSERT(false, "Cannot copy construct Fedora Agent");
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
			if (m_InputType == AgentInputType::PLAYER && m_Camera)
				m_Camera->Mode = CameraMode::NoClip;
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
			if (m_Camera)
				m_Camera->Mode = CameraMode::Pivot;
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
		m_FieldController = controller;
		controller->FrisbeeThrown.AddObserver(this);
		controller->FrisbeePickup.AddObserver(this);
	}
	// Uses field controller to set team reference
	void FedoraAgent::SetTeamColor(TeamColor color)
	{
		ASSERT(m_FieldController, "Must have field controller to set team");
		if (m_Team != nullptr)
			m_Team->RemoveAgent(this);
		m_Team = m_FieldController->GetTeam(color);
		m_SuitModel = Resources.GetModel((color == TeamColor::Blue ? "SuitBlue" : "SuitRed"));
		m_Team->AddAgent(this);
	}
	// Flattens and rotates agent to vector
	void FedoraAgent::SetFacingVector(Vector3 facing)
	{
		facing.y = 0;
		facing = glm::normalize(facing);
		float newYaw = glm::degrees(std::atan2(facing.z, facing.x)) - 90;
		ObjectTransform.SetYaw(newYaw);
		if (m_Camera && m_Camera->Mode == CameraMode::Pivot)
			m_Camera->LookInDirection(facing);
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
		return m_MaxSpeed * (IsInvincible() ? 1.25f : 1.0f);
	}
	Vector3 FedoraAgent::GetDirection() const
	{
		return m_Direction;
	}
	// Calculates risk rating based on how far the enemies are. Sum of squared distance to each enemy
	float FedoraAgent::CalculateRisk() const
	{
		ASSERT(m_FieldController != nullptr, "Must have field controller reference");
		Team* enemyTeam = m_FieldController->GetEnemyTeam(m_Team);
		return enemyTeam->CalculateRiskAtPos(ObjectTransform.Position);
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

	bool FedoraAgent::CanBeStolenFrom() const
	{
		return m_CanBeStolenFromTimer <= 0;
	}
	// Currently invincible if recently stolen fedora, may be decoupled later
	bool FedoraAgent::IsInvincible() const
	{
		return m_CanBeStolenFromTimer > 0 && IsPlayerControlled();
	}

	const Vector3 FedoraAgent::GetFuturePosition(float time) const
	{
		Vector3 futurePos = ObjectTransform.Position + m_Speed * m_Direction * time;
		return futurePos;
	}

	// Uses predicted future movement to throw fedora or to steal fedora
	Vector3 FedoraAgent::GetAgentPredictedPosition(Vector3 startingPoint, float interceptingSpeed) const
	{
		float dist = glm::length(ObjectTransform.Position - startingPoint);
		float timeForObjectToIntercept = dist / interceptingSpeed;
		Vector3 predictedAgentPos = GetFuturePosition(timeForObjectToIntercept);
		return predictedAgentPos;
	}

	Team * FedoraAgent::GetTeam() const
	{
		return m_Team;
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

		// Collision! - Currently doing manual checks
		#pragma region Collision
		if (m_FieldController != nullptr)
		{
			ASSERT(ObjectTransform.Position.x > -1000, "Object teleported out");
			Vector3 movement = ObjectTransform.Position - m_PrevPosition;
			Vector3 moveDir = glm::normalize(movement);

			// Walls
			std::vector<const GameObject*> walls = m_FieldController->GetCourt()->GetCollidingWalls(*this);
			if (!walls.empty())
			{
				const GameObject* wall = walls[0];
				ObjectTransform.Position = m_PrevPosition;
				Vector3 slidingDir = m_BoundingBox.GetSlidingDirection(ObjectTransform, wall->ObjectTransform, wall->GetBoundingBox(), m_Direction);
				ASSERT(slidingDir != Vector3(0.0f, 0.0f, 0.0f), "Sliding Dir should not be zero");
				ObjectTransform.Position += slidingDir * m_Speed * Game.DeltaTime();

				movement = slidingDir * m_Speed * Game.DeltaTime();
				moveDir = glm::normalize(movement);
				
				walls = m_FieldController->GetCourt()->GetCollidingWalls(*this);
				int i = 0;
				while (!walls.empty() && ++i < 1000)
				{
					Vector3 moveDir = glm::normalize(slidingDir);
					ObjectTransform.Position -= moveDir * 0.01f;
					walls = m_FieldController->GetCourt()->GetCollidingWalls(*this);
				}
			}
			// Agents
			{
				ASSERT(ObjectTransform.Position.x > -1000, "Object teleported out");
				// Do agent collision if not hitting boundary
				const FedoraAgent* other = m_FieldController->FindAgentCollidingAgent(this);
				// Lower timer if not inside agent
				if (other == nullptr)
					m_CanBeStolenFromTimer = glm::max(0.f, m_CanBeStolenFromTimer - Game.DeltaTime());

				Vector3 originalPosition = ObjectTransform.Position;

				int i = 0;
				while (other && ++i < 1000 && !IsInvincible())
				{
					//Steal Fedora
					if (other->GetHasFedora() && other->CanBeStolenFrom() && other->GetTeam() != m_Team)
					{
						// Send Frisbee Pickup Event
						FrisbeePickupEvent event(m_FieldController->GetFedoraPosition(), *this);
						m_FieldController->FrisbeePickup.Notify(event);

						m_CanBeStolenFromTimer = INVULNERABLE_TIME;
						m_Speed = GetMaxSpeed(); //speed boost

						m_FieldController->StunAgent(other, 2.0f);
					}


					float distToTouch = m_BoundingBox.GetOverlapDistance(ObjectTransform, other->ObjectTransform, other->m_BoundingBox, movement);

					// Change moved amount, still in same direction
					movement = moveDir * distToTouch;

					// Go Back In Time
					ObjectTransform.Position = m_PrevPosition;

					// Push Against Object
					ObjectTransform.Position += movement;

					// Still Hitting? Make Sure Not Hitting Anything
					const FedoraAgent* stillHitting = m_FieldController->FindAgentCollidingAgent(this);
					if (stillHitting) {
						other = stillHitting;
						movement = ObjectTransform.Position - m_PrevPosition;
						moveDir = glm::normalize(movement);
						continue;
					}

					// Sliding Movement
					Vector3 slidingDir = m_BoundingBox.GetSlidingDirection(ObjectTransform, other->ObjectTransform, other->m_BoundingBox, moveDir);

					if (slidingDir != Vector3(0.0f, 0.0f, 0.0f)) {
						// Reset Original Movement
						m_PrevPosition = ObjectTransform.Position;

						// Set new movement with new move direction
						movement = slidingDir * m_Speed * Game.DeltaTime();
						moveDir = glm::normalize(movement);

						ObjectTransform.Position += movement;


						// Find new colliding agent
						other = m_FieldController->FindAgentCollidingAgent(this);
					}
					else {
						// Cannot move any more
						other = nullptr;
					}
					

				}
				ASSERT(ObjectTransform.Position.x > -1000, "Object teleported out");
			}

			//  Back to Walls
			walls = m_FieldController->GetCourt()->GetCollidingWalls(*this);
			if (!walls.empty())
			{
				ObjectTransform.Position = m_PrevPosition;
				walls = m_FieldController->GetCourt()->GetCollidingWalls(*this);
			}
		}
		
		#pragma endregion

		if (m_InputComponent)
		{
			m_InputComponent->LateUpdate(this);
		}
	}
	// Upon game match reset
	void FedoraAgent::Reset()
	{
		m_CanGrabTimer = 0.f;
		m_CanBeStolenFromTimer = 0.f;
	}

	// Draws suit ontop of agent
	void FedoraAgent::DrawSuit() const
	{
		ShaderPtr shader = Resources.GetShader("Model");
		m_SuitModel->Draw(shader, this->ObjectTransform.GetMatrix());
	}

	#pragma region Event Handling
	// Handles Events about the match for this agent
	void FedoraAgent::OnEvent(const Subject* subject, Event& event)
	{
		Evnt::Dispatch<FrisbeeThrownEvent>(event, EVENT_BIND_FN(FedoraAgent, OnFrisbeeThrown));
		Evnt::Dispatch<FrisbeePickupEvent>(event, EVENT_BIND_FN(FedoraAgent, OnFrisbeePickup));
		if (m_InputComponent) {
			m_InputComponent->OnEvent(subject, event);
		}
	}
	// Reacts to when frisbee is thrown
	bool FedoraAgent::OnFrisbeeThrown(FrisbeeThrownEvent & e)
	{
		if (e.GetAgent().GetID() == GetID())
		{
			m_CanGrabTimer = 0.2f;
		}
		return false;
	}
	// Reacts to when frisbee is picked up
	bool FedoraAgent::OnFrisbeePickup(FrisbeePickupEvent & e)
	{
		if (e.GetAgent().GetID() == GetID())
		{
			m_CanGrabTimer = 0.1f;
			m_CanBeStolenFromTimer = 0.1f;
		}
		else {
			m_CanGrabTimer = 0.0f;
		}
		return false;
	}
	#pragma endregion
}
