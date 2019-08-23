#include <FedPCH.h>
#include <Resources/ResourceManager.h>s
#include "GameObject.h"

namespace Fed
{
	GameObject::GameObject()
	{
		//ASSERT(false, "GameObject must have model");
		m_ID = GetNextID();
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
	GameObject::GameObject(const GameObject & other)
	{
		ASSERT(false, "Cannot copy construct game object");
	}
	// Returns unique identifier for this object
	int GameObject::GetID() const
	{
		return m_ID;
	}
	// Sets transform relative to parameter's transform
	// Updates Position to be local to new parent
	// Can set to nullptr, to have no parent
	void GameObject::AttachToParent(const GameObject* parent)
	{
		if (ObjectTransform.Parent == &(parent->ObjectTransform))
			return;
		// Detach from current parent
		if (ObjectTransform.Parent)
		{
			ObjectTransform.Position += ObjectTransform.Parent->GetGlobalPosition();
			ObjectTransform.Rotation += ObjectTransform.Parent->GetGlobalRotation();
			ObjectTransform.Parent = nullptr;
		}
		// Update to new parent
		if (parent)
		{
			ObjectTransform.Parent = &(parent->ObjectTransform);
			ObjectTransform.Position -= parent->ObjectTransform.GetGlobalPosition();
			ObjectTransform.Rotation -= parent->ObjectTransform.GetGlobalRotation();
		}
		// Process Rotation
		ObjectTransform.Rotation = ProcessAngleVector(ObjectTransform.Rotation);
	}
	// Returns whether two bounding boxes intersect
	bool GameObject::IsColliding(const GameObject & other) const
	{
		return m_BoundingBox.IsIntersecting(ObjectTransform, other.ObjectTransform, other.m_BoundingBox);
	}
	bool GameObject::IsCollidingAtPosition(const GameObject& other, Vector3 newPos) const
	{
		Transform temp(ObjectTransform);
		temp.Position = newPos;
		return m_BoundingBox.IsIntersecting(temp, other.ObjectTransform, other.m_BoundingBox);
	}
	void GameObject::Draw()
	{
		if (m_Model == nullptr)
			return;
		ShaderPtr shader = Resources.GetShader("Model");
		Draw(shader);
	}
	void GameObject::Draw(const ShaderPtr & shader)
	{
		if (m_Model == nullptr)
			return;
		m_Model->Draw(shader, ObjectTransform.GetMatrix());
	}
	// Debug method to draw bounding box
	void GameObject::DrawBoundingBox() const
	{
		if (!m_BoundingBox.IsEmpty())
			m_BoundingBox.DebugDraw(ObjectTransform);
	}
	// Allows children to customize shape of bounding box
	void GameObject::SetBoundingBox(Vector3 center, Vector3 halfExtents)
	{
		m_BoundingBox.SetParameters(center, halfExtents);
	}
	BoundingBox GameObject::GetBoundingBox() const
	{
		return m_BoundingBox;
	}
	// Each object will have a unique identifier based on this static function
	int GameObject::GetNextID()
	{
		static int nextId = 0;
		return nextId++;
	}
}