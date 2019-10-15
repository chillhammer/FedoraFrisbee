#include "FedPCH.h"
#include "EndGameScreen.h"

#include "FedPCH.h"
#include <Resources/ResourceManager.h>
#include <EventSystem/Events/KeyEvent.h>
#include <EventSystem/Event.h>
#include <Game/States/GameStates.h>
#include <Audio/AudioEngine.h>
#include <Input/Keys.h>
#include "PauseScreen.h"

namespace Fed
{
	EndGameScreen::EndGameScreen() :
		m_FieldController(nullptr),
		m_Font(Resources.GetFont("InGame")),
		m_Backdrop(Vector4(0.0f, 0.0f, 0.0f, 0.5f)), m_Resume(), m_Exit(),
		m_Enabled(false)
	{
		float ww = Game.GetWindow().GetWidth();
		float wh = Game.GetWindow().GetHeight();
		m_Backdrop.UITransform.Scale = Vector3(ww, wh, 0.1f);
		m_Backdrop.UITransform.Position.z = 0.01f;
		Game.GetWindowResizedSubject().AddObserver(this);

		std::string header = (m_Won ? "You Won!" : "You Lost...");
		Vector2 headerSize = m_Font->GetTextSize(header, 2.0f);

		float bWidth = 400.0f;
		float bHeight = 100.0f;
		float bPad = 20.0f;
		float totalHeight = (bHeight + bPad) * 2.0f + (headerSize.y + bPad);
		float bx = ww * 0.5f - bWidth * 0.5f;
		float by = wh * 0.5f - totalHeight * 0.5f;


		Vector3 buttonScale = Vector3(bWidth, bHeight, 1.0f);
		float byy = by + headerSize.y + bPad;
		// Keep Playing
		m_Resume.SetProperties(bx, byy, bWidth, bHeight, "Keep Playing", [&] {
			m_Enabled = false;
			Game.GetWindow().SetCursorEnabled(false);
			Game.SetPause(false);
		});
		byy += bHeight + bPad;
		// Exit
		m_Exit.SetProperties(bx, byy, bWidth, bHeight, "Exit to Main Menu", [&] {
			m_Enabled = false;
			Game.SetPause(false);
			Game.ChangeState(GameStates::MainMenu::Instance());
		});
	}
	bool EndGameScreen::IsEnabled()
	{
		return m_Enabled;
	}
	void EndGameScreen::UpdateAndDraw()
	{
		if (!m_Enabled) return;

		m_Backdrop.Draw();

		// Sloppy near publishing code-duplication
		std::string header = (m_Won ? "You Won!" : "You Lost...");
		Vector2 headerSize = m_Font->GetTextSize(header, 1.f);

		float ww = Game.GetWindow().GetWidth();
		float wh = Game.GetWindow().GetHeight();
		float bWidth = 400.0f;
		float bHeight = 100.0f;
		float bPad = 20.0f;
		float totalHeight = (bHeight + bPad) * 2.0f + (headerSize.y + bPad);

		m_Font->RenderText(header, (Vector2(ww, wh) * 0.5f) - (headerSize * 0.5f) - Vector2(0.0f, totalHeight * 0.5f), 1.f, Vector3(1.0f, 1.0f, 1.0f));

		m_Resume.UpdateAndDraw();
		m_Exit.UpdateAndDraw();
	}
	void EndGameScreen::SetFieldControllerReference(FrisbeeFieldController* controller)
	{
		if (m_FieldController != nullptr)
			return;
		m_FieldController = controller;
		m_FieldController->GameEnded.AddObserver(this);
	}

	void EndGameScreen::OnEvent(const Subject* subject, Event& event)
	{
		Evnt::Dispatch<GameEndedEvent>(event, EVENT_BIND_FN(EndGameScreen, OnGameEnded));
		Evnt::Dispatch<WindowResizeEvent>(event, EVENT_BIND_FN(EndGameScreen, OnWindowResized));
	}
	bool EndGameScreen::OnGameEnded(GameEndedEvent& e)
	{
		m_Enabled = true;
		Game.SetPause(true);
		auto [bScore, rScore] = e.GetScores();
		m_Won = bScore > rScore;
		Game.GetWindow().SetCursorEnabled(m_Enabled);
		return false;
	}
	// Makes the screen scale. Note: currently duplicating hard-coded spacing values
	bool EndGameScreen::OnWindowResized(WindowResizeEvent& e)
	{
		float ww = Game.GetWindow().GetWidth();
		float wh = Game.GetWindow().GetHeight();

		// Update Positions
		m_Backdrop.UITransform.Scale = Vector3(ww, wh, 0.1f);

		std::string header = (m_Won ? "You Won!" : "You Lost...");
		Vector2 headerSize = m_Font->GetTextSize(header, 1.f);

		float bWidth = 400.0f;
		float bHeight = 100.0f;
		float bPad = 20.0f;
		float totalHeight = (bHeight + bPad) * 2.f + (headerSize.y + bPad);
		float bx = ww * 0.5f - bWidth * 0.5f;
		float by = wh * 0.5f - totalHeight * 0.5f;

		Vector3 buttonScale = Vector3(bWidth, bHeight, 1.0f);
		float byy = by + headerSize.y + bPad;
		// Resume
		m_Resume.UITransform.Position = Vector3(bx, byy, 0.1f);

		byy += bHeight + bPad;
		// Exit
		m_Exit.UITransform.Position = Vector3(bx, byy, 0.1f);

		return false;
	}
}

