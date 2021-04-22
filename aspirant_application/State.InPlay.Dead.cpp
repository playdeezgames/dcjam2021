#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.Texts.h"
#include "Game.Avatar.Items.h"
#include <sstream>
#include "States.h"
#include "Application.MouseButtonUp.h"
namespace state::in_play::Dead
{
	const std::string LAYOUT_NAME = "State.InPlay.Dead";
	const std::string JOOLS_TEXT_ID = "Jools";

	static void GoToMainMenu()
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
	}

	static void OnUpdate(const Uint32&)
	{
		std::stringstream ss;
		ss << "You collected " << game::avatar::Items::Read(5) << " jools!";//TODO: hardcoded
		graphics::Texts::SetText(LAYOUT_NAME, JOOLS_TEXT_ID, ss.str());
	}

	void OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8 buttons)
	{
		GoToMainMenu();
	}

	void Start()
	{
		::application::MouseButtonUp::SetHandler(::UIState::IN_PLAY_DEAD, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_DEAD, GoToMainMenu);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_DEAD, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_DEAD, OnUpdate);
	}
}
