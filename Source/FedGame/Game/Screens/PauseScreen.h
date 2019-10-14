#pragma once
#include <Game/GameManager.h>
#include <Input/InputManager.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include <UI/Button.h>
#include <Graphics/Text/Font.h>

namespace Fed
{
	class PauseScreen : public IObserver
	{
	public:
		PauseScreen();

		void ToggleScreen();
		bool IsEnabled();
		void UpdateAndDraw();
		void SetFieldControllerReference(FrisbeeFieldController* controller);

		virtual void OnEvent(const Subject* subject, class Event& event) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		FrisbeeFieldController* m_FieldController;
		FontPtr m_Font;
		UIElement m_Backdrop;
		Button m_Resume;
		Button m_Takeover;
		Button m_Exit;
		bool m_Enabled;
	};
}