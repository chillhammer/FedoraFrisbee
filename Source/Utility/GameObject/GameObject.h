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
		int GetID() const;
		void AttachToParent(GameObject* parent);
		void Draw();
		void Draw(const ShaderPtr& shader);
		void DrawBoundingBox();
	public:
		Transform ObjectTransform;
	protected:
		void SetBoundingBox(Vector3 center, Vector3 halfExtents);
	protected:
		ModelPtr m_Model;
	private:
		BoundingBox m_BoundingBox;
		static int GetNextID();
		int m_ID;
	};
}