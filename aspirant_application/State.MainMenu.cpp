#include "Application.Renderer.h"
#include "Visuals.Layouts.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Visuals.Menus.h"
#include "Common.Utility.h"
#include "Application.MouseMotion.h"
#include "Application.MouseButtonUp.h"
#include "Visuals.Areas.h"
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
		common::Utility::Dispatch(activators, (MainMenuItem)visuals::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
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
		visuals::Menus::WriteValue(LAYOUT_NAME, MENU_ID, (int)item);
	}

	static void OnMouseMotionInArea(const std::string& area)
	{
		SetCurrentMenuItem(areaMenuItems.find(area)->second);
	}

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		ActivateItem();
		return true;
	}

	std::function<bool(const common::XY<Sint32>&, Uint8)> HandleMouseButtonUp(const std::string& layoutName, std::function<bool(const std::string&)> areaHandler)
	{
		return [layoutName, areaHandler](const common::XY<Sint32>& xy, Uint8)
		{
			auto areas = visuals::Areas::Get(layoutName, xy);
			for (auto& area : areas)
			{
				if (areaHandler(area))
				{
					return true;
				}
			}
			return false;
		};
	}

	void Start()
	{
		::application::MouseMotion::AddHandler(::UIState::MAIN_MENU, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea));
		::application::MouseButtonUp::AddHandler(::UIState::MAIN_MENU, HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandlers(::UIState::MAIN_MENU, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::MAIN_MENU, LAYOUT_NAME);
	}
}
