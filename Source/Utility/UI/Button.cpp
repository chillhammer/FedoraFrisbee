#include "FedPCH.h"
#include <Resources/ResourceManager.h>
#include <Input/InputManager.h>
#include "Button.h"

namespace Fed {
	Button::Button() : UIElement(Vector4(0.0f, 0.0f, 0.0f, 1.0f)), m_OnClick()
	{
		Input.MouseClicked.AddObserver(this);
	}
	Button::Button(float x, float y, float width, float height, std::string label, std::function<void(void)>& onClick) : UIElement(Vector4(0.0f, 0.0f, 0.0f, 1.0f))
	{
		SetProperties(x, y, width, height, label, onClick);
		Input.MouseClicked.AddObserver(this);
	}
	void Button::SetProperties(float x, float y, float width, float height, std::string label, std::function<void(void)> onClick)
	{
		m_OnClick = onClick;
		UITransform.Position = { x, y, 0.0f };
		UITransform.Scale = { width, height, 1.0f };
		m_Label = label;
	}
	void Button::SetLabel(std::string label)
	{
		m_Label = label;
	}
	void Button::UpdateAndDraw()
	{
		FontPtr font = Resources.GetFont("Arial");
		Vector2 topLeft = UITransform.Position;
		Vector2 bottomRight = UITransform.Position + UITransform.Scale;
		Vector2 middle = UITransform.Position + UITransform.Scale * 0.5f;
		Vector2 textDims = font->GetTextSize(m_Label, 1.0f);

		m_Sprite.m_Color = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		m_IsHovering = false;
		Vector2 mPos = Input.GetMousePosition();

		if (topLeft.x < mPos.x && topLeft.y < mPos.y && bottomRight.x > mPos.x && bottomRight.y > mPos.y) {
			float brightness = 0.2f;
			m_Sprite.m_Color += Vector4(brightness, brightness, brightness, 0.0f);
			m_IsHovering = true;
		}

		Draw();
		font->RenderText(m_Label, middle - textDims * 0.5f, 1.0f, Vector3(1.0f, 1.0f, 1.0f));
	}
	void Button::OnEvent(const Subject* subject, Event& event)
	{
		Evnt::Dispatch<MouseButtonPressedEvent>(event, EVENT_BIND_FN(Button, OnMouseClick));
	}
	bool Button::OnMouseClick(MouseButtonPressedEvent& e)
	{
		if (m_IsHovering) {
			m_OnClick();
			m_IsHovering = false;
		}
		return false;
	}
}
