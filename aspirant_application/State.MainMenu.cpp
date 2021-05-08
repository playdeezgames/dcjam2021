#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.Renderer.h"
#include "Common.Utility.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
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

	static void GoToAbout()
	{
		SDL_SetClipboardText("https://thegrumpygamedev.itch.io/"); 
		::application::UIState::Write(::UIState::ABOUT);
	}

	const std::map<MainMenuItem, std::function<void()>> activators =
	{
		{ MainMenuItem::START, ::application::UIState::GoTo(::UIState::START_GAME) },
		{ MainMenuItem::OPTIONS, ::application::UIState::GoTo(::UIState::OPTIONS)  },
		{ MainMenuItem::ABOUT, GoToAbout },
		{ MainMenuItem::QUIT, ::application::UIState::GoTo(::UIState::CONFIRM_QUIT)  },
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (MainMenuItem)visuals::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::CONFIRM_QUIT) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::CONFIRM_QUIT) }
	};

	static bool OnMouseButtonUpInArea(const std::string&)
	{
		ActivateItem();
		return true;
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
		visuals::Menus::WriteValue(LAYOUT_NAME, MENU_ID, (int)item);
	}

	static void OnMouseMotionInArea(const std::string& area)
	{
		SetCurrentMenuItem(areaMenuItems.find(area)->second);
	}

	void Start()
	{
		::application::MouseMotion::AddHandler(::UIState::MAIN_MENU, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea));
		::application::MouseButtonUp::AddHandler(::UIState::MAIN_MENU, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandlers(::UIState::MAIN_MENU, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::MAIN_MENU, LAYOUT_NAME);
	}
}