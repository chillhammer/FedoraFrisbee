#pragma once
#include <Transform/Transform.h>
#include <Utility/Graphics/Sprite/Sprite.h>

namespace Fed 
{
	class UIElement {
	public:
		UIElement(std::string textureName);
		UIElement(Vector4 color);
		Transform UITransform;

		void Draw(const ShaderPtr& shader);
		void Draw();
	private:
		UIElement() {};
		UIElement(const UIElement& other) {};
	protected:
		Sprite m_Sprite;
	};
}