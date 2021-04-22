#include "Application.Renderer.h"
#include "Graphics.Layouts.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Graphics.Menus.h"
#include "Common.Utility.h"
#include "Application.MouseMotion.h"
#include "Application.MouseButtonUp.h"
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

	static void StartGame()
	{
		::application::UIState::Write(::UIState::START_GAME);
	}

	static void GoToOptions()
	{
		::application::UIState::Write(::UIState::OPTIONS);
	}

	static void GoToAbout()
	{
		SDL_SetClipboardText("https://thegrumpygamedev.itch.io/"); 
		::application::UIState::Write(::UIState::ABOUT);
	}

	static void ConfirmQuit()
	{
		::application::UIState::Write(::UIState::CONFIRM_QUIT);
	}

	const std::map<MainMenuItem, std::function<void()>> activators =
	{
		{ MainMenuItem::START, StartGame },
		{ MainMenuItem::OPTIONS, GoToOptions },
		{ MainMenuItem::ABOUT, GoToAbout },
		{ MainMenuItem::QUIT, ConfirmQuit },
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (MainMenuItem)graphics::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
	}

	static void NextMenuItem()
	{
		graphics::Menus::Next(LAYOUT_NAME, MENU_ID);
	}

	static void PreviousMenuItem()
	{
		graphics::Menus::Previous(LAYOUT_NAME, MENU_ID);
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, PreviousMenuItem },
		{::Command::DOWN, NextMenuItem },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ConfirmQuit },
		{::Command::RED, ConfirmQuit }
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

	static void OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8)
	{
		auto areas = graphics::Areas::Get(LAYOUT_NAME, xy);
		if (!areas.empty())
		{
			ActivateItem();
		}
	}

	void Start()
	{
		::application::MouseButtonUp::SetHandler(::UIState::MAIN_MENU, OnMouseButtonUp);
		::application::MouseMotion::SetHandler(::UIState::MAIN_MENU, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::MAIN_MENU, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::MAIN_MENU, LAYOUT_NAME);
	}
}
