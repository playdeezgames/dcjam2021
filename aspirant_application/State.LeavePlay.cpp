#include <string>
#include "Visuals.Menus.h"
#include "Application.UIState.h"
#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
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
		common::Utility::Dispatch(activators, (LeavePlayItem)visuals::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
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
		visuals::Menus::WriteValue(LAYOUT_NAME, MENU_ID, (int)item);
	}

	const std::map<std::string, LeavePlayItem> areaMenuItems =
	{
		{ AREA_CONTINUE,  LeavePlayItem::CONTINUE},
		{ AREA_SAVE,  LeavePlayItem::SAVE},
		{ AREA_ABANDON,  LeavePlayItem::ABANDON}
	};

	static void OnMouseMotion(const common::XY<Sint32>& xy)//TODO: make an MouseMotionArea handler?
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		for (auto& area : areas)
		{
			SetCurrentMenuItem(areaMenuItems.find(area)->second);
		}
	}

	static bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8)//TODO: duplicated code with other menus
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		if (!areas.empty())
		{
			ActivateItem();
			return true;
		}
		return false;
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::LEAVE_PLAY, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::LEAVE_PLAY, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::LEAVE_PLAY, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::LEAVE_PLAY, LAYOUT_NAME);
	}
}