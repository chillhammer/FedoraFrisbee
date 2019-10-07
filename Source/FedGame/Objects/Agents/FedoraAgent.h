#pragma once
#include <GameObject/GameObject.h>
#include <EventSystem/IObserver.h>
#include <FrisbeeFieldController/Team/Team.h>
#include "Components/FedoraAgentInputComponent.h"

namespace Fed
{
	enum AgentInputType {NONE, AI, PLAYER};
	/**
		Agent used to represent fedora frisbee player.
		Either AI or human player.
	**/
	class FedoraAgent 
		: public GameObject, public IObserver
	{
		friend class FedoraAgentInputPlayer;
		friend class FedoraAgentInputAI;
	public:
		FedoraAgent();
		FedoraAgent(const FedoraAgent& other);
		virtual ~FedoraAgent();
		void SetInputType(AgentInputType inputType);
		void SetCameraReference(class Camera* camera);
		void SetFieldControllerReference(class FrisbeeFieldController* controller);
		void SetTeamColor(TeamColor color);
		void SetFacingVector(Vector3 facing);

		class FrisbeeFieldController* GetFieldController() const;
		bool	GetHasFedora() const;
		float	GetMaxSpeed() const;
		Vector3 GetDirection() const;
		float	CalculateRisk() const;
		bool	InFedoraPath() const;
		bool	IsPlayerControlled() const;
		bool	CanBeStolenFrom() const;
		const Vector3 GetFuturePosition(float time) const;
		Vector3 GetAgentPredictedPosition(Vector3 startingPoint, float interceptingSpeed) const;
		Team* GetTeam() const;

		void DrawSuit() const;
		void OnEvent(const Subject* subject, Event& event) override;
		void Update();
		void Reset();
	private:
		bool OnFrisbeeThrown(class FrisbeeThrownEvent& e);
		bool OnFrisbeePickup(class FrisbeePickupEvent& e);
	private:
		AgentInputType m_InputType;
		FedoraAgentInputComponent* m_InputComponent;
		Team* m_Team;
		ModelPtr m_SuitModel;
		class Camera* m_Camera;
		class FrisbeeFieldController* m_FieldController;
		float m_Speed;
		float m_MaxSpeed;
		Vector3 m_Direction;
		Vector3 m_PrevPosition;
		float m_CanGrabTimer;
		float m_CanBeStolenFromTimer;
		const float INVULNERABLE_TIME = 1.0f;
	};
}