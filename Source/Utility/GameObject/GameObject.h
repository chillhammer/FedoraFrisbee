#pragma once
#include <Transform/Transform.h>
#include <Graphics/Model/Model.h>
#include "BoundingBox.h"

namespace Fed
{
	/**
		Base class for all game objects in the game.
		Each has a unique identifier to check if they are the same
	**/
	class GameObject
	{
	public:
		GameObject();
		GameObject(std::string modelName);
		GameObject(std::string modelName, Vector3 position);
		GameObject(const GameObject& other);
		int GetID() const;
		void AttachToParent(const GameObject* parent);
		bool IsColliding(const GameObject& other) const;
		bool IsCollidingAtPosition(const GameObject& other, Vector3 newPos) const;
		void Draw();
		void Draw(const ShaderPtr& shader);
		void DrawBoundingBox() const;
	public:
		Transform ObjectTransform;
		void SetBoundingBox(Vector3 center, Vector3 halfExtents);
		BoundingBox GetBoundingBox() const;
	protected:
		ModelPtr m_Model;
		BoundingBox m_BoundingBox;
	private:
		static int GetNextID();
		int m_ID;
	};
}