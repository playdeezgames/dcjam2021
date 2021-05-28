#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Visuals.Menus.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
namespace state::ConfirmQuit
{
	const std::string LAYOUT_NAME = "State.ConfirmQuit";
	const std::string MENU_ID = "ConfirmQuit";
	const std::string AREA_NO = "No";
	const std::string AREA_YES = "Yes";

	enum class ConfirmQuitItem
	{
		NO,
		YES
	};

	const std::map<ConfirmQuitItem, std::function<void()>> activators =
	{
		{ ConfirmQuitItem::NO, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ConfirmQuitItem::YES, ::application::UIState::GoTo(::UIState::QUIT) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (ConfirmQuitItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::MAIN_MENU) }
	};

	static void SetCurrentMenuItem(ConfirmQuitItem item)
	{
		visuals::Menus::WriteIndex(LAYOUT_NAME, MENU_ID, (int)item);
	}

	const std::map<std::string, ConfirmQuitItem> areaMenuItems =
	{
		{ AREA_NO,  ConfirmQuitItem::NO},
		{ AREA_YES,  ConfirmQuitItem::YES}
	};

	static bool OnMouseButtonUpInArea(const std::string&)
	{
		ActivateItem();
		return true;
	}

	static void OnMouseMotionInArea(const std::string& area, const common::XY<int>&)
	{
		SetCurrentMenuItem(areaMenuItems.find(area)->second);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::CONFIRM_QUIT, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::CONFIRM_QUIT, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(::UIState::CONFIRM_QUIT, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea));
		::application::Command::SetHandlers(::UIState::CONFIRM_QUIT, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::CONFIRM_QUIT, LAYOUT_NAME);
	}
}