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

	static void ActivateItem()
	{
		switch ((MainMenuItem)graphics::Menus::Read(LAYOUT_NAME, MENU_ID).value())
		{
		case MainMenuItem::START:
			::application::UIState::Write(::UIState::START_GAME);
			return;
		case MainMenuItem::OPTIONS:
			::application::UIState::Write(::UIState::OPTIONS);
			return;
		case MainMenuItem::ABOUT:
			SDL_SetClipboardText("https://thegrumpygamedev.itch.io/");
			::application::UIState::Write(::UIState::ABOUT);
			return;
		case MainMenuItem::QUIT:
			::application::UIState::Write(::UIState::CONFIRM_QUIT);
			return;
		}
	}

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::UP:
			graphics::Menus::Previous(LAYOUT_NAME, MENU_ID);
			break;
		case ::Command::DOWN:
			graphics::Menus::Next(LAYOUT_NAME, MENU_ID);
			break;
		case ::Command::GREEN:
			ActivateItem();
			break;
		case ::Command::BACK:
		case ::Command::RED:
			::application::UIState::Write(::UIState::CONFIRM_QUIT);
			break;
		}
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::MAIN_MENU, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::MAIN_MENU, LAYOUT_NAME);
	}
}
