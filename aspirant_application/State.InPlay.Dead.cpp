#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Visuals.Texts.h"
#include "Game.Avatar.Items.h"
#include <sstream>
#include "States.h"
#include "Application.MouseButtonUp.h"
#include "Visuals.Layouts.h"
#include "Game.Avatar.h"
#include "Common.Audio.h"
#include "Game.Audio.Mux.h"
namespace state::in_play::Dead
{
	const std::string LAYOUT_NAME = "State.InPlay.Dead";
	const std::string JOOLS_TEXT_ID = "Jools";
	const std::string TROUSERS_TEXT_ID = "Trousers";

	static void OnEnter()
	{
		std::stringstream ss;
		ss << "You collected " << game::avatar::Items::Read(game::Avatar::GetDescriptor().joolsItemId) << " jools!";
		visuals::Texts::SetText(LAYOUT_NAME, JOOLS_TEXT_ID, ss.str());
		if (game::avatar::Items::Read(game::Avatar::GetDescriptor().trousersItemId)>0)
		{
			visuals::Texts::SetText(LAYOUT_NAME, TROUSERS_TEXT_ID, "...at least you kept yer dignity!");
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

	bool OnMouseButtonUp(const common::XY<int>& xy, Uint8 buttons)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_DEAD, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_DEAD, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_DEAD, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_DEAD, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_DEAD, OnEnter);
	}
}
