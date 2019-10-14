#include "FedPCH.h"
#include <Resources/ResourceManager.h>
#include "UIElement.h"

namespace Fed {
	UIElement::UIElement(std::string textureName) : m_Sprite(Resources.GetTexture(textureName))
	{
	}
	UIElement::UIElement(Vector4 color) : m_Sprite(color)
	{
	}
	void UIElement::Draw(const ShaderPtr& shader)
	{
		m_Sprite.Draw(shader, UITransform.GetMatrix());
	}

	void UIElement::Draw()
	{
		Draw(Resources.GetShader("UI"));
	}

}