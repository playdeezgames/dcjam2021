#include "Application.Renderer.h"
#include "Graphics.Layouts.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Graphics.Menus.h"
#include "Common.Utility.h"
#include "Application.MouseMotion.h"
#include "Graphics.Areas.h"
namespace state::MainMenu
{
	const std::string LAYOUT_NAME = "State.MainMenu";
	const std::string MENU_ID = "Main";
	const std::string AREA_START = "Start";
	const std::string AREA_ABOUT = "About";
	const std::string AREA_OPTIONS = "Options";
	const std::string AREA_QUIT = "Quit";

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
		common::Utility::Dispatch(activators, (MainMenuItem)graphics::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
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
		common::Utility::Dispatch(commandHandlers, command);
	}

	const std::map<std::string, MainMenuItem> areaMenuItems =
	{
		{ AREA_START,  MainMenuItem::START},
		{ AREA_ABOUT,  MainMenuItem::ABOUT},
		{ AREA_OPTIONS,  MainMenuItem::OPTIONS},
		{ AREA_QUIT,  MainMenuItem::QUIT}
	};

	static void SetCurrentMenuItem(MainMenuItem item) 
	{ 
		graphics::Menus::WriteIndex(LAYOUT_NAME, MENU_ID, 
			graphics::Menus::FindIndexForValue(LAYOUT_NAME, MENU_ID, (int)item).value()); 
	}

	static void OnMouseMotion(const common::XY<Sint32>& xy)
	{
		auto areas = graphics::Areas::Get(LAYOUT_NAME, xy);
		for (auto& area : areas)
		{
			SetCurrentMenuItem(areaMenuItems.find(area)->second);
		}
	}

	void Start()
	{
		::application::MouseMotion::SetHandler(::UIState::MAIN_MENU, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::MAIN_MENU, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::MAIN_MENU, LAYOUT_NAME);
	}
}
