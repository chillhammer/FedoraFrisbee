#pragma once
#include <Game/GameManager.h>
#include <Input/InputManager.h>
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include <UI/Button.h>
#include <Graphics/Text/Font.h>

namespace Fed
{
	class EndGameScreen : public IObserver
	{
	public:
		EndGameScreen();

		bool IsEnabled();
		void UpdateAndDraw();
		void SetFieldControllerReference(FrisbeeFieldController* controller);

		virtual void OnEvent(const Subject* subject, class Event& event) override;

	private:
		bool OnGameEnded(GameEndedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		FrisbeeFieldController* m_FieldController;
		FontPtr m_Font;
		UIElement m_Backdrop;
		Button m_Resume;
		Button m_Exit;
		bool m_Enabled;
		bool m_Won;
	};
}