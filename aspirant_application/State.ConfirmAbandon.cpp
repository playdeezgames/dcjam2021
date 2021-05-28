#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Visuals.Menus.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
#include "Common.Audio.h"
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
namespace state::ConfirmAbandon
{
	const std::string LAYOUT_NAME = "State.ConfirmAbandon";
	const std::string MENU_ID = "ConfirmAbandon";
	const std::string AREA_NO = "No";
	const std::string AREA_YES = "Yes";

	enum class ConfirmAbandonItem
	{
		NO,
		YES
	};

	static void CancelAbandon()
	{
		common::audio::Sfx::Play(application::UIState::EnterGame());
	}

	const std::map<ConfirmAbandonItem, std::function<void()>> activators =
	{
		{ ConfirmAbandonItem::NO, CancelAbandon },
		{ ConfirmAbandonItem::YES, ::application::UIState::GoTo(::UIState::MAIN_MENU) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (ConfirmAbandonItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::MAIN_MENU) }
	};

	static void SetCurrentMenuItem(ConfirmAbandonItem item)
	{
		visuals::Menus::WriteIndex(LAYOUT_NAME, MENU_ID, (int)item);
	}

	const std::map<std::string, ConfirmAbandonItem> areaMenuItems =
	{
		{ AREA_NO,  ConfirmAbandonItem::NO},
		{ AREA_YES,  ConfirmAbandonItem::YES}
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
		::application::OnEnter::AddHandler(::UIState::CONFIRM_ABANDON, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::CONFIRM_ABANDON, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(::UIState::CONFIRM_ABANDON, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea));
		::application::Command::SetHandlers(::UIState::CONFIRM_ABANDON, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::CONFIRM_ABANDON, LAYOUT_NAME);
	}
}