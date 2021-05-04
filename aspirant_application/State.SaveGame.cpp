#include "Application.Renderer.h"
#include "Visuals.Layouts.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Visuals.Menus.h"
#include "Game.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Application.OnEnter.h"
#include "Visuals.Areas.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
namespace state::SaveGame
{
	const std::string LAYOUT_NAME = "State.SaveGame";
	const std::string MENU_ID = "SaveGame";
	const std::string MENU_ITEM_AUTOSAVE = "Autosave";
	const std::string MENU_ITEM_SLOT1 = "Slot1";
	const std::string MENU_ITEM_SLOT2 = "Slot2";
	const std::string MENU_ITEM_SLOT3 = "Slot3";
	const std::string MENU_ITEM_SLOT4 = "Slot4";
	const std::string MENU_ITEM_SLOT5 = "Slot5";
	const std::string AUTOSAVE_PRESENT = "(autosave)";
	const std::string NOT_PRESENT = "-";
	const std::string AREA_AUTOSAVE = "Autosave";
	const std::string AREA_SLOT1 = "Slot1";
	const std::string AREA_SLOT2 = "Slot2";
	const std::string AREA_SLOT3 = "Slot3";
	const std::string AREA_SLOT4 = "Slot4";
	const std::string AREA_SLOT5 = "Slot5";
	const std::string AREA_BACK = "Back";
	const std::string SLOT_PRESENT_1 = "Slot 1";
	const std::string SLOT_PRESENT_2 = "Slot 2";
	const std::string SLOT_PRESENT_3 = "Slot 3";
	const std::string SLOT_PRESENT_4 = "Slot 4";
	const std::string SLOT_PRESENT_5 = "Slot 5";

	enum class SaveGameItem
	{
		AUTOSAVE,
		SLOT_1,
		SLOT_2,
		SLOT_3,
		SLOT_4,
		SLOT_5,
		BACK
	};

	static void GoBack()
	{
		::application::UIState::Write(::UIState::LEAVE_PLAY);
	}

	static void SaveToAutosave()
	{
		game::AutoSave();
		application::UIState::EnterGame();
	}

	static std::function<void()> SlotSaver(int slot)
	{
		return [slot]()
		{
			game::SaveToSlot(slot);
			application::UIState::EnterGame();
		};
	}

	const std::map<SaveGameItem, std::function<void()>> activators =
	{
		{ SaveGameItem::AUTOSAVE, SaveToAutosave },
		{ SaveGameItem::SLOT_1, SlotSaver(1) },
		{ SaveGameItem::SLOT_2, SlotSaver(2) },
		{ SaveGameItem::SLOT_3, SlotSaver(3) },
		{ SaveGameItem::SLOT_4, SlotSaver(4) },
		{ SaveGameItem::SLOT_5, SlotSaver(5) },
		{ SaveGameItem::BACK, GoBack }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (SaveGameItem)visuals::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::BACK, GoBack },
		{ ::Command::RED, GoBack },
		{ ::Command::GREEN, ActivateItem }
	};

	static void UpdateMenuItem(bool shown, const std::string& menuItem, const std::string& textWhenShown)
	{
		if (shown)
		{
			visuals::MenuItems::SetText(LAYOUT_NAME, menuItem, textWhenShown);
		}
		else
		{
			visuals::MenuItems::SetText(LAYOUT_NAME, menuItem, NOT_PRESENT);
		}
	}

	static void OnEnter()
	{
		UpdateMenuItem(game::DoesAutosaveExist(), MENU_ITEM_AUTOSAVE, AUTOSAVE_PRESENT);
		UpdateMenuItem(game::DoesSlotExist(1), MENU_ITEM_SLOT1, SLOT_PRESENT_1);
		UpdateMenuItem(game::DoesSlotExist(2), MENU_ITEM_SLOT2, SLOT_PRESENT_2);
		UpdateMenuItem(game::DoesSlotExist(3), MENU_ITEM_SLOT3, SLOT_PRESENT_3);
		UpdateMenuItem(game::DoesSlotExist(4), MENU_ITEM_SLOT4, SLOT_PRESENT_4);
		UpdateMenuItem(game::DoesSlotExist(5), MENU_ITEM_SLOT5, SLOT_PRESENT_5);
	}

	const std::map<std::string, SaveGameItem> areaMenuItems =
	{
		{ AREA_AUTOSAVE,  SaveGameItem::AUTOSAVE},
		{ AREA_SLOT1,  SaveGameItem::SLOT_1},
		{ AREA_SLOT2,  SaveGameItem::SLOT_2},
		{ AREA_SLOT3,  SaveGameItem::SLOT_3},
		{ AREA_SLOT4,  SaveGameItem::SLOT_4},
		{ AREA_SLOT5,  SaveGameItem::SLOT_5},
		{ AREA_BACK,  SaveGameItem::BACK}
	};

	static void SetCurrentMenuItem(SaveGameItem item)
	{
		visuals::Menus::WriteValue(LAYOUT_NAME, MENU_ID, (int)item);
	}

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
		::application::MouseButtonUp::AddHandler(::UIState::SAVE_GAME, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::SAVE_GAME, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::SAVE_GAME, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::SAVE_GAME, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::SAVE_GAME, OnEnter);
	}
}