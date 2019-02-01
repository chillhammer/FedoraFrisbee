#pragma once
#include <GameObject/GameObject.h>
#include "Components/FedoraAgentInputComponent.h"

namespace Fed
{
	enum AgentInputType {NONE, AI, PLAYER};
	/**
		Agent used to represent fedora frisbee player.
		Either AI or human player.
	**/
	class FedoraAgent 
		: public GameObject
	{
		friend class FedoraAgentInputPlayer;
	public:
		FedoraAgent();
		FedoraAgent(Vector3 position);
		virtual ~FedoraAgent();
		void SetInputType(AgentInputType inputType);
		void SetCameraReference(class Camera* camera);
		void Update();
	private:
		AgentInputType m_InputType;
		FedoraAgentInputComponent* m_InputComponent;
		class Camera* m_Camera;
	};
}