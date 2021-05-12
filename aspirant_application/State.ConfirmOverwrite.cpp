#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Visuals.Menus.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
#include "Common.Audio.h"
#include "Game.h"
namespace state::ConfirmOverwrite
{
	const std::string LAYOUT_NAME = "State.ConfirmOverwrite";
	const std::string MENU_ID = "ConfirmOverwrite";
	const std::string AREA_NO = "No";
	const std::string AREA_YES = "Yes";

	enum class ConfirmAbandonItem
	{
		NO,
		YES
	};

	const std::map<::UIState, int> slotTable =
	{
		{::UIState::CONFIRM_OVERWRITE_SLOT1, 1},
		{::UIState::CONFIRM_OVERWRITE_SLOT2, 2},
		{::UIState::CONFIRM_OVERWRITE_SLOT3, 3},
		{::UIState::CONFIRM_OVERWRITE_SLOT4, 4},
		{::UIState::CONFIRM_OVERWRITE_SLOT5, 5},
	};

	static void ConfirmOverwrite()
	{
		int slot = slotTable.find(application::UIState::Read())->second;
		game::SaveToSlot(slot);
		application::UIState::EnterGame();
	}

	const std::map<ConfirmAbandonItem, std::function<void()>> activators =
	{
		{ ConfirmAbandonItem::NO, application::UIState::GoTo(::UIState::SAVE_GAME) },
		{ ConfirmAbandonItem::YES, ConfirmOverwrite }
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

	static bool OnMouseButtonUpInArea(const std::string&)
	{
		ActivateItem();
		return true;
	}

	static void OnMouseMotionInArea(const std::string& area, const common::XY<Sint32>&)
	{
		SetCurrentMenuItem(areaMenuItems.find(area)->second);
	}

	const std::vector<::UIState> states =
	{
		::UIState::CONFIRM_OVERWRITE_SLOT1,
		::UIState::CONFIRM_OVERWRITE_SLOT2,
		::UIState::CONFIRM_OVERWRITE_SLOT3,
		::UIState::CONFIRM_OVERWRITE_SLOT4,
		::UIState::CONFIRM_OVERWRITE_SLOT5,
	};

	void Start()
	{
		for (auto state : states)
		{
			::application::MouseButtonUp::AddHandler(state, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
			::application::MouseMotion::AddHandler(state, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea));
			::application::Command::SetHandlers(state, commandHandlers);
			::application::Renderer::SetRenderLayout(state, LAYOUT_NAME);
		}
	}
}