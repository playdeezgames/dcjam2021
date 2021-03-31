#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Graphics.Layouts.h"
#include "Graphics.Menus.h"
namespace state::ConfirmQuit
{
	const std::string LAYOUT_NAME = "State.ConfirmQuit";
	const std::string MENU_ID = "ConfirmQuit";

	enum class ConfirmQuitItem
	{
		NO,
		YES
	};

	static void ActivateItem()
	{
		switch ((ConfirmQuitItem)graphics::Menus::Read(LAYOUT_NAME,MENU_ID).value())
		{
		case ConfirmQuitItem::NO:
			::application::UIState::Write(::UIState::MAIN_MENU);
			return;
		case ConfirmQuitItem::YES:
			::application::UIState::Write(::UIState::QUIT);
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
			::application::UIState::Write(::UIState::MAIN_MENU);
			break;
		}
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::CONFIRM_QUIT, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::CONFIRM_QUIT, LAYOUT_NAME);
	}
}