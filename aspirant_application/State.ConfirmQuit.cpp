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

	static void GoToMainMenu()
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
	}

	static void Quit()
	{
		::application::UIState::Write(::UIState::QUIT);
	}

	const std::map<ConfirmQuitItem, std::function<void()>> activators =
	{
		{ ConfirmQuitItem::NO, GoToMainMenu },
		{ ConfirmQuitItem::YES, Quit }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (ConfirmQuitItem)graphics::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
	}

	static void PreviousMenuItem()
	{
		graphics::Menus::Previous(LAYOUT_NAME, MENU_ID);
	}

	static void NextMenuItem()
	{
		graphics::Menus::Next(LAYOUT_NAME, MENU_ID);
	}

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, PreviousMenuItem },
		{ ::Command::DOWN, NextMenuItem },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, GoToMainMenu },
		{ ::Command::RED, GoToMainMenu }
	};

	void Start()
	{
		::application::Command::SetHandlers(::UIState::CONFIRM_QUIT, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::CONFIRM_QUIT, LAYOUT_NAME);
	}
}