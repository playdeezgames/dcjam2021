#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Graphics.Layouts.h"
#include "Graphics.Menus.h"
#include "Common.Utility.h"
namespace state::ConfirmQuit
{
	const std::string LAYOUT_NAME = "State.ConfirmQuit";
	const std::string MENU_ID = "ConfirmQuit";

	enum class ConfirmQuitItem
	{
		NO,
		YES
	};

	const std::map<ConfirmQuitItem, std::function<void()>> activators =
	{
		{ ConfirmQuitItem::NO, []() { ::application::UIState::Write(::UIState::MAIN_MENU); }},
		{ ConfirmQuitItem::YES, []() { ::application::UIState::Write(::UIState::QUIT); }}
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (ConfirmQuitItem)graphics::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<Command, std::function<void()>> commandHandlers =
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
		::application::Command::SetHandler(::UIState::CONFIRM_QUIT, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::CONFIRM_QUIT, LAYOUT_NAME);
	}
}