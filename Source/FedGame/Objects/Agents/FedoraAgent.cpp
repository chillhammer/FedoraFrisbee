#include <FedPCH.h>
#include <Resources/ResourceManager.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include <Game/GameManager.h>
#include <EventSystem/Events/FrisbeeFieldEvent.h>
#include <EventSystem/IObserver.h>
#include <Audio/AudioEngine.h>
#include "FedoraAgent.h"
#include "Components/FedoraAgentInputPlayer.h"
#include "Components/FedoraAgentInputComponent.h"
#include "Components/FedoraAgentInputAI.h"

namespace
{
	constexpr float AGENT_GROUND_Y = 0.0f;
}

namespace Fed
{
	FedoraAgent::FedoraAgent() 
		: GameObject("FedoraAgent"), m_InputType(AgentInputType::NONE), m_InputComponent(nullptr), m_Team(nullptr),
			m_SuitModel(Resources.GetModel("SuitBlue")), m_Camera(nullptr), m_FieldController(nullptr), m_Speed(0.0f), m_MaxSpeed(10.75f),
			m_Direction(0.0f, 0.0f, 1.0f), m_PrevPosition(0.0f, 0.0f, 0.0f), m_CanGrabTimer(0.f), m_CanBeStolenFromTimer(0.0f),
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
		facing = SafeNormalize(facing, Vector3(0.0f, 0.0f, 1.0f));
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
		futurePos.y = AGENT_GROUND_Y;
		return futurePos;
	}

	// Uses predicted future movement to throw fedora or to steal fedora
	Vector3 FedoraAgent::GetAgentPredictedPosition(Vector3 startingPoint, float interceptingSpeed) const
	{
		if (interceptingSpeed <= 0.000001f || !std::isfinite(interceptingSpeed))
			return ObjectTransform.Position;
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
		if (!IsFiniteVector(ObjectTransform.Position))
		{
			ObjectTransform.Position = IsFiniteVector(m_PrevPosition) ? m_PrevPosition : Vector3(0.0f, 0.0f, 0.0f);
			m_Speed = 0.0f;
			m_Direction = SafeNormalize(m_Direction, Vector3(0.0f, 0.0f, 1.0f));
		}
		KeepOnGround();

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
			movement.y = 0.0f;
			if (!IsFiniteVector(movement))
				movement = Vector3(0.0f, 0.0f, 0.0f);
			Vector3 moveDir = SafeNormalize(movement, SafeNormalize(m_Direction, Vector3(1.0f, 0.0f, 0.0f)));
			moveDir.y = 0.0f;

			// Walls
			std::vector<const GameObject*> walls = m_FieldController->GetCourt()->GetCollidingWalls(*this);
			if (!walls.empty())
			{
				const GameObject* wall = walls[0];
				float distToTouch = m_BoundingBox.GetOverlapDistance(ObjectTransform, wall->ObjectTransform, wall->GetBoundingBox(), movement);

				ObjectTransform.Position = m_PrevPosition;			// Move out
				ObjectTransform.Position += moveDir * distToTouch * 0.99f;	// Push against wall
				KeepOnGround();

				Vector3 slidingDir = m_BoundingBox.GetSlidingDirection(ObjectTransform, wall->ObjectTransform, wall->GetBoundingBox(), m_Direction);
				slidingDir.y = 0.0f;
				ASSERT(slidingDir != Vector3(0.0f, 0.0f, 0.0f), "Sliding Dir should not be zero");
				ObjectTransform.Position += slidingDir * m_Speed * Game.DeltaTime();
				KeepOnGround();

				movement = slidingDir * m_Speed * Game.DeltaTime();
				movement.y = 0.0f;
				moveDir = SafeNormalize(movement, moveDir);
				moveDir.y = 0.0f;
				
				walls = m_FieldController->GetCourt()->GetCollidingWalls(*this);
				int i = 0;
				while (!walls.empty() && ++i < 1000)
				{
					Vector3 moveDir = SafeNormalize(slidingDir, Vector3(1.0f, 0.0f, 0.0f));
					moveDir.y = 0.0f;
					ObjectTransform.Position -= moveDir * 0.01f;
					KeepOnGround();
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
						Audio.PlaySound3D("Stole.wav", ObjectTransform.Position);
					}


					if (!IsFiniteVector(movement) || glm::length2(movement) <= 0.000001f)
					{
						Vector3 pushDir = SafeNormalize(ObjectTransform.Position - other->ObjectTransform.Position, moveDir);
						pushDir.y = 0.0f;
						pushDir = SafeNormalize(pushDir, moveDir);
						int pushIterations = 0;
						while (other && ++pushIterations < 100)
						{
							ObjectTransform.Position += pushDir * 0.02f;
							KeepOnGround();
							other = m_FieldController->FindAgentCollidingAgent(this);
						}
						m_PrevPosition = ObjectTransform.Position;
						movement = Vector3(0.0f, 0.0f, 0.0f);
						moveDir = pushDir;
						continue;
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
						movement.y = 0.0f;
						moveDir = SafeNormalize(movement, moveDir);
						moveDir.y = 0.0f;
						continue;
					}

					// Sliding Movement
					Vector3 slidingDir = m_BoundingBox.GetSlidingDirection(ObjectTransform, other->ObjectTransform, other->m_BoundingBox, moveDir);
					slidingDir.y = 0.0f;

					if (slidingDir != Vector3(0.0f, 0.0f, 0.0f)) {
						// Reset Original Movement
						m_PrevPosition = ObjectTransform.Position;
						m_PrevPosition.y = AGENT_GROUND_Y;

						// Set new movement with new move direction
						movement = slidingDir * m_Speed * Game.DeltaTime();
						movement.y = 0.0f;
						moveDir = SafeNormalize(movement, moveDir);
						moveDir.y = 0.0f;

						ObjectTransform.Position += movement;
						KeepOnGround();


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
				KeepOnGround();
				walls = m_FieldController->GetCourt()->GetCollidingWalls(*this);
			}
		}
		
		#pragma endregion
		if (!IsFiniteVector(ObjectTransform.Position))
		{
			ObjectTransform.Position = IsFiniteVector(m_PrevPosition) ? m_PrevPosition : Vector3(0.0f, 0.0f, 0.0f);
			m_Speed = 0.0f;
		}
		KeepOnGround();

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
		m_Speed = 0.0f;
		if (!IsFiniteVector(ObjectTransform.Position))
			ObjectTransform.Position = Vector3(0.0f, 0.0f, 0.0f);
		if (!IsFiniteVector(ObjectTransform.Rotation))
			ObjectTransform.Rotation = Vector3(0.0f, 0.0f, 0.0f);
		m_Direction = SafeNormalize(ObjectTransform.GetHeading(), Vector3(0.0f, 0.0f, 1.0f));
		m_Direction.y = 0.0f;
		m_Direction = SafeNormalize(m_Direction, Vector3(0.0f, 0.0f, 1.0f));
		KeepOnGround();
		m_PrevPosition = ObjectTransform.Position;
	}

	void FedoraAgent::KeepOnGround()
	{
		ObjectTransform.Position.y = AGENT_GROUND_Y;
		m_PrevPosition.y = AGENT_GROUND_Y;
		m_Direction.y = 0.0f;
		m_Direction = SafeNormalize(m_Direction, Vector3(0.0f, 0.0f, 1.0f));
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
			Audio.PlaySound3D("Pickup.wav", ObjectTransform.Position);
		}
		else {
			m_CanGrabTimer = 0.0f;
		}
		return false;
	}
	#pragma endregion
}
