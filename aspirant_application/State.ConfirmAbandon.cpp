#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Visuals.Layouts.h"
#include "Visuals.Menus.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
#include "Common.Audio.h"
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

	static void ConfirmAbandon()
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
	}

	static void CancelAbandon()
	{
		common::audio::Sfx::Play(application::UIState::EnterGame());
	}

	const std::map<ConfirmAbandonItem, std::function<void()>> activators =
	{
		{ ConfirmAbandonItem::NO, CancelAbandon },
		{ ConfirmAbandonItem::YES, ConfirmAbandon }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (ConfirmAbandonItem)visuals::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
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
		visuals::Menus::WriteValue(LAYOUT_NAME, MENU_ID, (int)item);
	}

	const std::map<std::string, ConfirmAbandonItem> areaMenuItems =
	{
		{ AREA_NO,  ConfirmAbandonItem::NO},
		{ AREA_YES,  ConfirmAbandonItem::YES}
	};

	static void OnMouseMotion(const common::XY<Sint32>& xy)//TODO: make an MouseMotionArea handler?
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		for (auto& area : areas)
		{
			SetCurrentMenuItem(areaMenuItems.find(area)->second);
		}
	}

	bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8)//TODO: duplicated code with other menus
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
		::application::MouseButtonUp::AddHandler(::UIState::CONFIRM_ABANDON, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::CONFIRM_ABANDON, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::CONFIRM_ABANDON, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::CONFIRM_ABANDON, LAYOUT_NAME);
	}
}