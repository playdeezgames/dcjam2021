#include "Application.Renderer.h"
#include "Graphics.Layouts.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Graphics.Menus.h"
#include "Game.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Application.OnEnter.h"
#include "Graphics.MenuItems.h"
namespace state::LoadGame
{
	const std::string LAYOUT_NAME = "State.LoadGame";
	const std::string MENU_ID = "LoadGame";
	const std::string MENU_ITEM_AUTOSAVE = "Autosave";
	//const std::string MENU_ITEM_SLOT1 = "Slot1";
	//const std::string MENU_ITEM_SLOT2 = "Slot2";
	//const std::string MENU_ITEM_SLOT3 = "Slot3";
	//const std::string MENU_ITEM_SLOT4 = "Slot4";
	//const std::string MENU_ITEM_SLOT5 = "Slot5";

	enum class LoadGameItem
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
		::application::UIState::Write(::UIState::START_GAME);
	}

	const std::map<LoadGameItem, std::function<void()>> activators =
	{
		{ LoadGameItem::BACK, GoBack }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (LoadGameItem)graphics::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, []() { graphics::Menus::Previous(LAYOUT_NAME, MENU_ID); }},
		{ ::Command::DOWN, []() { graphics::Menus::Next(LAYOUT_NAME, MENU_ID); }},
		{ ::Command::BACK, GoBack },
		{ ::Command::RED, GoBack },
		{ ::Command::GREEN, ActivateItem }
	};

	static void OnEnter()
	{
		graphics::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_AUTOSAVE, "(autosave)");
	}

	void Start()
	{
		::application::Command::SetHandlers(::UIState::LOAD_GAME, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::LOAD_GAME, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::LOAD_GAME, OnEnter);
	}
}