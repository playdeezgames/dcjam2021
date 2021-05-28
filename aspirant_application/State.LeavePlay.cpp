#include "Visuals.Menus.h"
#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
namespace state::LeavePlay
{
	const std::string LAYOUT_NAME = "State.LeavePlay";
	const std::string MENU_ID = "LeavePlay";
	const std::string AREA_CONTINUE = "Continue";
	const std::string AREA_ABANDON = "Abandon";
	const std::string AREA_SAVE = "Save";

	enum class LeavePlayItem
	{
		CONTINUE,
		SAVE,
		ABANDON
	};

	static void GoToConfirmAbandon()
	{
		::application::UIState::Write(::UIState::CONFIRM_ABANDON);
	}

	static void GoToSaveGame()
	{
		::application::UIState::Write(::UIState::SAVE_GAME);
	}

	static void ContinueGame()
	{
		common::audio::Sfx::Play(application::UIState::EnterGame());
	}

	const std::map<LeavePlayItem, std::function<void()>> activators =
	{
		{ LeavePlayItem::ABANDON, GoToConfirmAbandon },
		{ LeavePlayItem::SAVE, GoToSaveGame },
		{ LeavePlayItem::CONTINUE, ContinueGame }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (LeavePlayItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, ContinueGame },
		{ ::Command::RED, ContinueGame }
	};

	static void SetCurrentMenuItem(LeavePlayItem item)
	{
		visuals::Menus::WriteIndex(LAYOUT_NAME, MENU_ID, (int)item);
	}

	const std::map<std::string, LeavePlayItem> areaMenuItems =
	{
		{ AREA_CONTINUE,  LeavePlayItem::CONTINUE},
		{ AREA_SAVE,  LeavePlayItem::SAVE},
		{ AREA_ABANDON,  LeavePlayItem::ABANDON}
	};

	static void OnMouseMotionInArea(const std::string& area, const common::XY<int>&)
	{
		SetCurrentMenuItem(areaMenuItems.find(area)->second);
	}

	static bool OnMouseButtonUpInArea(const std::string&)
	{
		ActivateItem();
		return true;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::LEAVE_PLAY, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::LEAVE_PLAY, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(::UIState::LEAVE_PLAY, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea));
		::application::Command::SetHandlers(::UIState::LEAVE_PLAY, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::LEAVE_PLAY, LAYOUT_NAME);
	}
}