#include <FedPCH.h>
#include <Resources/ResourceManager.h>s
#include "GameObject.h"

namespace Fed
{
	GameObject::GameObject()
	{
		ASSERT(false, "GameObject must have model");
	}
	GameObject::GameObject(std::string modelName) 
		: m_Model(Resources.GetModel(modelName))
	{
		m_ID = GetNextID();
	}
	GameObject::GameObject(std::string modelName, Vector3 position)
		: m_Model(Resources.GetModel(modelName))
	{
		ObjectTransform.Position = position;
		m_ID = GetNextID();
	}
	// Returns unique identifier for this object
	int GameObject::GetID() const
	{
		return m_ID;
	}
	// Sets transform relative to parameter's transform
	void GameObject::AttachToParent(GameObject* parent)
	{
		ASSERT(parent, "Cannot attach transform to nullptr");
		ObjectTransform.Parent = &(parent->ObjectTransform);
	}
	void GameObject::Draw()
	{
		ShaderPtr shader = Resources.GetShader("Model");
		Draw(shader);
	}
	void GameObject::Draw(const ShaderPtr & shader)
	{
		m_Model->Draw(shader, ObjectTransform.GetMatrix());
	}
	// Each object will have a unique identifier based on this static function
	int GameObject::GetNextID()
	{
		static int nextId = 0;
		return nextId++;
	}
}