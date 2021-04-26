#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.Texts.h"
#include "Game.Avatar.Items.h"
#include <sstream>
#include "Application.MouseButtonUp.h"
namespace state::in_play::Exit
{
	const std::string LAYOUT_NAME = "State.InPlay.Exit";
	const std::string JOOLS_TEXT_ID = "Jools";
	const std::string TROUSERS_TEXT_ID = "Jools";

	static void GoToMainMenu()
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
	}

	static void OnUpdate(const Uint32&)
	{
		std::stringstream ss;
		ss << "You collected " << game::avatar::Items::Read(5) << " jools!";
		graphics::Texts::SetText(LAYOUT_NAME, JOOLS_TEXT_ID, ss.str());
		if (game::avatar::Items::Read(6) > 0)//TODO: hardcoded
		{
			graphics::Texts::SetText(LAYOUT_NAME, TROUSERS_TEXT_ID, "...you managed to keep yer trousers!");
		}
		else
		{
			graphics::Texts::SetText(LAYOUT_NAME, TROUSERS_TEXT_ID, "...you lost yer trousers(again)!");
		}

	}

	bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8 buttons)
	{
		GoToMainMenu();
		return true;
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_EXIT, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_EXIT, GoToMainMenu);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_EXIT, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_EXIT, OnUpdate);
	}
}
