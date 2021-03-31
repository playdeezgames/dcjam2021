#include "Application.Renderer.h"
#include "Graphics.Layouts.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Graphics.Menus.h"
namespace state::Start
{
	const std::string LAYOUT_NAME = "State.Start";
	const std::string MENU_ID = "Start";

	enum class StartGameItem
	{
		EDITOR,
		PLAY,
		BACK
	};

	static void ActivateItem()
	{
		switch ((StartGameItem)graphics::Menus::Read(LAYOUT_NAME, MENU_ID).value())
		{
		case StartGameItem::BACK:
			::application::UIState::Write(::UIState::MAIN_MENU);
			break;
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
		case ::Command::BACK:
		case ::Command::RED:
			::application::UIState::Write(::UIState::MAIN_MENU);
			break;
		case ::Command::GREEN:
			ActivateItem();
			break;
		}
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::START_GAME, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::START_GAME, LAYOUT_NAME);
	}
}