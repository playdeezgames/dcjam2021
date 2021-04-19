#include "Application.Renderer.h"
#include "Graphics.Layouts.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Graphics.Menus.h"
#include "Game.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
namespace state::Start
{
	const std::string LAYOUT_NAME = "State.Start";
	const std::string MENU_ID = "Start";

	enum class StartGameItem
	{
		NEW_GAME,
		BACK
	};

	static void NewGame()
	{
		game::Reset();
		common::audio::Sfx::Play(application::UIState::EnterGame());
	}

	static void GoBack()
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
	}

	const std::map<StartGameItem, std::function<void()>> activators =
	{
		{ StartGameItem::NEW_GAME, NewGame },
		{ StartGameItem::BACK, GoBack }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (StartGameItem)graphics::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, []() { graphics::Menus::Previous(LAYOUT_NAME, MENU_ID); }},
		{ ::Command::DOWN, []() { graphics::Menus::Next(LAYOUT_NAME, MENU_ID); }},
		{ ::Command::BACK, []() { ::application::UIState::Write(::UIState::MAIN_MENU); }},
		{ ::Command::RED, []() { ::application::UIState::Write(::UIState::MAIN_MENU); }},
		{ ::Command::GREEN, ActivateItem }
	};

	static void OnCommand(const ::Command& command)
	{
		common::Utility::Dispatch(commandHandlers, command);
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::START_GAME, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::START_GAME, LAYOUT_NAME);
	}
}