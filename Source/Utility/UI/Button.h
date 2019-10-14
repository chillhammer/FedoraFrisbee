#pragma once
#include <EventSystem/Events/MouseEvent.h>
#include "UIElement.h"

namespace Fed
{
	class Button : public UIElement, IObserver
	{
	public:
		Button();
		Button(float x, float y, float width, float height, std::string label, std::function<void(void)>& onClick);
		void SetProperties(float x, float y, float width, float height, std::string label, std::function<void(void)> onClick);
		void SetLabel(std::string label);
		void UpdateAndDraw();
		void OnEvent(const Subject* subject, class Event& event);
	private:
		bool OnMouseClick(MouseButtonPressedEvent& e);
		std::function<void(void)> m_OnClick;
		std::string m_Label;
		bool m_IsHovering = false;
	};
}