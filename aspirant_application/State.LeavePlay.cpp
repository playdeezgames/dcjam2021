#include <string>
#include "Graphics.Menus.h"
#include "Application.UIState.h"
#include "Command.h"
#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
namespace state::LeavePlay
{
	const std::string LAYOUT_NAME = "State.LeavePlay";
	const std::string MENU_ID = "LeavePlay";

	enum class LeavePlayItem
	{
		CONTINUE,
		ABANDON
	};

	const std::map<LeavePlayItem, std::function<void()>> activators =
	{
		{ LeavePlayItem::ABANDON, []() {::application::UIState::Write(::UIState::MAIN_MENU); }},
		{ LeavePlayItem::CONTINUE, []() {common::audio::Sfx::Play(application::UIState::EnterGame()); }}
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (LeavePlayItem)graphics::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, []() { graphics::Menus::Previous(LAYOUT_NAME, MENU_ID); }},
		{ ::Command::DOWN, []() { graphics::Menus::Next(LAYOUT_NAME, MENU_ID); }},
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, []() { ::application::UIState::Write(::UIState::MAIN_MENU); }},
		{ ::Command::RED, []() { ::application::UIState::Write(::UIState::MAIN_MENU); }}
	};

	static void OnCommand(const ::Command& command)
	{
		common::Utility::Dispatch(commandHandlers, command);
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::LEAVE_PLAY, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::LEAVE_PLAY, LAYOUT_NAME);
	}
}