#pragma once
#include <Transform/Transform.h>
#include <Graphics/Model/Model.h>

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
	public:
		Transform ObjectTransform;
	protected:
		ModelPtr m_Model;
	private:
		static int GetNextID();
		int m_ID;
	};
}