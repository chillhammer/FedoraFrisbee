#pragma once
#include <GameObject/GameObject.h>
#include <EventSystem/IObserver.h>
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
	public:
		FedoraAgent();
		virtual ~FedoraAgent();
		void SetInputType(AgentInputType inputType);
		void SetCameraReference(class Camera* camera);
		void SetFieldControllerReference(class FrisbeeFieldController* controller);

		class FrisbeeFieldController* GetFieldController() const;
		bool GetHasFedora() const;
		bool InFedoraPath() const;

		// bool IsColliding(Fedora* fedora);

		void OnEvent(const Subject* subject, Event& event) override;
		void Update();
	private:
		bool OnFrisbeeThrown(class FrisbeeThrownEvent& e);
		bool OnFrisbeePickup(class FrisbeePickupEvent& e);
	private:
		AgentInputType m_InputType;
		FedoraAgentInputComponent* m_InputComponent;
		class Camera* m_Camera;
		class FrisbeeFieldController* m_FieldController;
		float m_Speed;
		float m_MaxSpeed;
		Vector3 m_Direction;
		Vector3 m_PrevPosition;
		float m_CanGrabTimer;
	};
}