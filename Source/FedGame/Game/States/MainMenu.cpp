#include <FedPCH.h>
#include <Input/InputManager.h>
#include <Resources/ResourceManager.h>
#include <Utility/UI/UIElement.h>
#include <Objects/Box/BoxMesh.h>
#include <Audio/AudioEngine.h>
#include "GameStates.h"

namespace Fed::GameStates
{
	// Main Menu State
	void MainMenu::Enter(GameManager* owner)
	{
		Game.GetWindow().SetCursorEnabled(true);
		m_UIShader = Resources.GetShader("UI");
		m_UIShader->Bind();
		m_UIShader->SetUniformMat4f("u_Projection", Camera::GetOrthographicMatrix());

		m_Start.SetProperties(0.0f, 0.0f, 200.0f, 75.0f, std::string("Start"), [] {
			Game.ChangeState(GameStates::TeamMatch::Instance());
		});
		Audio.PlayMusic("MainMenu.wav");
	}

	void MainMenu::Execute(GameManager* owner)
	{
		m_UIShader->Bind();
		m_UIShader->SetUniformMat4f("u_Projection", Camera::GetOrthographicMatrix());

		m_Start.UITransform.Position = Vector3(Game.GetWindow().GetWidth() * 0.6f, Game.GetWindow().GetHeight() * 0.8f, 0.1f);

		UIElement titleScreen("TitleScreen");
		titleScreen.UITransform.Scale = Vector3(Game.GetWindow().GetWidth(), Game.GetWindow().GetHeight(), 0.0f);
		titleScreen.Draw(m_UIShader);


		m_Start.UpdateAndDraw();
	}

	void MainMenu::Exit(GameManager* owner)
	{
		Audio.PlayMusic("MainTheme.wav");
	}
}