#include <string>
#include "Graphics.Menus.h"
#include "Application.UIState.h"
#include "Command.h"
#include "Application.Command.h"
#include "Application.Renderer.h"
namespace state::LeavePlay
{
	const std::string LAYOUT_NAME = "State.LeavePlay";
	const std::string MENU_ID = "LeavePlay";

	enum class LeavePlayItem
	{
		CONTINUE,
		ABANDON
	};

	static void ActivateItem()
	{
		switch ((LeavePlayItem)graphics::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value())
		{
		case LeavePlayItem::ABANDON:
			::application::UIState::Write(::UIState::MAIN_MENU);
			return;
		case LeavePlayItem::CONTINUE:
			::application::UIState::EnterGame();
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
		::application::Command::SetHandler(::UIState::LEAVE_PLAY, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::LEAVE_PLAY, LAYOUT_NAME);
	}
}