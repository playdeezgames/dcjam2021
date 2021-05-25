#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Visuals.Texts.h"
#include "Game.Avatar.Items.h"
#include <sstream>
#include "Application.MouseButtonUp.h"
#include "Game.Avatar.h"
#include "Visuals.Layouts.h"
#include "Game.Audio.Mux.h"
namespace state::in_play::Exit
{
	const std::string LAYOUT_NAME = "State.InPlay.Exit";
	const std::string JOOLS_TEXT_ID = "Jools";
	const std::string TROUSERS_TEXT_ID = "Jools";

	static void OnEnter()
	{
		std::stringstream ss;
		ss << "You collected " << game::avatar::Items::Read(game::Avatar::GetDescriptor().joolsItemId) << " jools!";
		visuals::Texts::SetText(LAYOUT_NAME, JOOLS_TEXT_ID, ss.str());
		if (game::avatar::Items::Read(game::Avatar::GetDescriptor().trousersItemId) > 0)
		{
			visuals::Texts::SetText(LAYOUT_NAME, TROUSERS_TEXT_ID, "...you managed to keep yer trousers!");
		}
		else if (game::avatar::Items::Read(game::Avatar::GetDescriptor().soiledTrousersItemId) > 0)
		{
			visuals::Texts::SetText(LAYOUT_NAME, TROUSERS_TEXT_ID, "...and you pooped yer pants!");
		}
		else
		{
			visuals::Texts::SetText(LAYOUT_NAME, TROUSERS_TEXT_ID, "...you lost yer trousers(again)!");
		}
	}

	bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8 buttons)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::ABOUT, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_EXIT, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_EXIT, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_EXIT, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_EXIT, OnEnter);
	}
}
