#include "Application.Renderer.h"
#include "Graphics.Layouts.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Graphics.Menus.h"
namespace state::MainMenu
{
	const std::string LAYOUT_NAME = "State.MainMenu";
	const std::string MENU_ID = "Main";

	enum class MainMenuItem
	{
		START,
		ABOUT,
		OPTIONS,
		QUIT
	};

	const std::map<MainMenuItem, std::function<void()>> activators =
	{
		{ MainMenuItem::START, []() {::application::UIState::Write(::UIState::START_GAME); }},
		{ MainMenuItem::OPTIONS, []() { ::application::UIState::Write(::UIState::OPTIONS); }},
		{ MainMenuItem::ABOUT, []() { SDL_SetClipboardText("https://thegrumpygamedev.itch.io/"); ::application::UIState::Write(::UIState::ABOUT); }},
		{ MainMenuItem::QUIT, []() { ::application::UIState::Write(::UIState::CONFIRM_QUIT); }},
	};

	static void ActivateItem()
	{
		auto iter = activators.find((MainMenuItem)graphics::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
		if (iter != activators.end())
		{
			iter->second();
		}
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, []() { graphics::Menus::Previous(LAYOUT_NAME, MENU_ID); }},
		{::Command::DOWN, []() { graphics::Menus::Next(LAYOUT_NAME, MENU_ID); }},
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, []() { ::application::UIState::Write(::UIState::CONFIRM_QUIT); }},
		{::Command::RED, []() { ::application::UIState::Write(::UIState::CONFIRM_QUIT); }}
	};

	static void OnCommand(const ::Command& command)
	{
		auto iter = commandHandlers.find(command);
		if (iter != commandHandlers.end())
		{
			iter->second();
		}
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::MAIN_MENU, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::MAIN_MENU, LAYOUT_NAME);
	}
}
