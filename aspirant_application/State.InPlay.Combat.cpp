#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Game.Combat.h"
#include "Visuals.Images.h"
#include "Visuals.Menus.h"
#include "Game.Avatar.h"
#include "Common.RNG.h"
#include "Common.Audio.h"
#include "Visuals.AvatarInventory.h"
#include <sstream>
#include "Common.Audio.h"
#include "Application.Sounds.h"
#include "Game.Creatures.h"
#include "Game.Avatar.Items.h"
#include "Game.CombatDeck.h"
#include "Visuals.Texts.h"
#include "Visuals.CardSprites.h"
#include "Game.Avatar.Items.h"
#include "Game.Item.h"
#include "Common.Utility.h"
#include "Game.Creatures.h"
#include "Visuals.Areas.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Data.Colors.h"
#include "Application.OnEnter.h"
#include "Visuals.Layouts.h"
namespace state::in_play::Combat
{
	const std::string LAYOUT_NAME = "State.InPlay.Combat";
	const std::string INVENTORY_LAYOUT_NAME = "State.InPlay.AvatarInventory";
	const std::string CONTROL_AVATAR_INVENTORY = "AvatarInventory";
	const std::string CURRENT_CARD_IMAGE_ID = "CurrentCard";
	const std::string COMBAT_MENU_ID = "Combat";
	const std::string USE_ITEM_MENU_ITEM_ID = "UseItem";

	const std::string AREA_GUESS_HIGHER = "GuessHigher";
	const std::string AREA_GUESS_LOWER = "GuessLower";
	const std::string AREA_USE_ITEM = "UseItem";
	const std::string AREA_NEXT_ITEM = "NextItem";
	const std::string AREA_PREVIOUS_ITEM = "PreviousItem";
	const std::string AREA_RUN_AWAY = "RunAway";
	const std::string TEXT_NEXT_ITEM = "NextItem";
	const std::string TEXT_PREVIOUS_ITEM = "PreviousItem";

	enum class CombatMenuItem
	{
		HIGHER,
		LOWER,
		USE_ITEM,
		RUN_AWAY
	};

	static void Flee()
	{
		auto count = common::RNG::FromRange(0,(int)maze::Directions::All().size());
		while (count > 0)
		{
			game::Avatar::TurnRight();
			count--;
		}
		game::Avatar::MoveAhead();
	}

	const std::string COMBATRESULT_LAYOUT_NAME = "State.InPlay.CombatResult";
	const std::string COMBATRESULT_RESULT_TEXT_ID = "Result";
	const std::string COMBATRESULT_HIT_MONSTER = "You hit it!";
	const std::string COMBATRESULT_KILL_MONSTER = "You killed it!";
	const std::string COMBATRESULT_GOT_HIT = "It hit you!";
	const std::string COMBATRESULT_BLOCKED_HIT = "You block!";
	const std::string COMBATRESULT_MISSED_MONSTER = "It blocked!";
	const std::string COMBATRESULT_HUNTER_RAN = "You attempted to run!";
	const std::string COMBATRESULT_MONSTER_DISTRACTED = "It's distracted!";

	void SetCombatResultText(const std::string& text)
	{
		visuals::Texts::SetText(COMBATRESULT_LAYOUT_NAME, COMBATRESULT_RESULT_TEXT_ID, text);
	}

	const std::map<game::Combat::CombatResult, std::string> resolutions =
	{
		{game::Combat::CombatResult::MONSTER_KILLED, COMBATRESULT_KILL_MONSTER},
		{game::Combat::CombatResult::MONSTER_HIT, COMBATRESULT_HIT_MONSTER},
		{game::Combat::CombatResult::MONSTER_BLOCKED, COMBATRESULT_MISSED_MONSTER},
		{game::Combat::CombatResult::HUNTER_HIT, COMBATRESULT_GOT_HIT},
		{game::Combat::CombatResult::HUNTER_BLOCKED, COMBATRESULT_BLOCKED_HIT},
		{game::Combat::CombatResult::HUNTER_RAN, COMBATRESULT_HUNTER_RAN},
		{game::Combat::CombatResult::MONSTER_DISTRACTED, COMBATRESULT_MONSTER_DISTRACTED}
	};

	const std::map<game::Combat::CombatResult, std::function<std::optional<std::string>(const std::optional<game::Creatures::Instance>&)>> sounds =
	{
		{game::Combat::CombatResult::MONSTER_KILLED, [](const std::optional<game::Creatures::Instance>& instance) { return instance.value().descriptor.sfx.find(game::creature::Sfx::DEATH)->second; }},
		{game::Combat::CombatResult::MONSTER_HIT, [](const std::optional<game::Creatures::Instance>& instance) { return instance.value().descriptor.sfx.find(game::creature::Sfx::HIT)->second; }},
		{game::Combat::CombatResult::MONSTER_BLOCKED, [](const std::optional<game::Creatures::Instance>& instance) { return instance.value().descriptor.sfx.find(game::creature::Sfx::BLOCK)->second; }},
		{game::Combat::CombatResult::HUNTER_HIT, [](const std::optional<game::Creatures::Instance>&) { return application::Sounds::Read(application::UI::Sfx::HIT_HUNTER); }},
		{game::Combat::CombatResult::HUNTER_BLOCKED, [](const std::optional<game::Creatures::Instance>&) { return application::Sounds::Read(application::UI::Sfx::HIT_BLOCKED); }},
		{game::Combat::CombatResult::HUNTER_RAN, [](const std::optional<game::Creatures::Instance>&) { return application::Sounds::Read(application::UI::Sfx::HIT_HUNTER); }},
		{game::Combat::CombatResult::MONSTER_DISTRACTED, [](const std::optional<game::Creatures::Instance>&) { return application::Sounds::Read(application::UI::Sfx::HIT_BLOCKED); }}
	};

	void ResolveCombat(std::optional<game::CombatDeck::Guess> guess)
	{
		auto result = game::Combat::Resolve(guess);
		auto& resolutionDetails = resolutions.find(result)->second;
		SetCombatResultText(resolutionDetails);
		auto instance = game::Creatures::GetInstance(game::Avatar::GetPosition());
		common::audio::Sfx::Play(sounds.find(result)->second(instance));
	}

	static void GuessHigher()
	{
		ResolveCombat(game::CombatDeck::Guess::HIGHER);
		application::UIState::Write(::UIState::IN_PLAY_COMBAT_RESULT);
	}

	static void GuessLower()
	{
		ResolveCombat(game::CombatDeck::Guess::LOWER);
		application::UIState::Write(::UIState::IN_PLAY_COMBAT_RESULT);
	}

	static void RunAway()
	{
		ResolveCombat(std::nullopt);
		Flee();
	}

	static void UseItem()
	{
		auto result = game::avatar::Items::CombatUse(visuals::AvatarInventory::GetItem(INVENTORY_LAYOUT_NAME, CONTROL_AVATAR_INVENTORY));
		if (result)
		{
			common::audio::Sfx::Play(std::get<0>(*result));
			if (std::get<1>(*result)== game::avatar::Items::CombatUseResult::REFRESH)
			{
				common::audio::Sfx::Play(application::UIState::EnterGame());
			}
		}
	}

	const std::map<CombatMenuItem, std::function<void()>> activators =
	{
		{CombatMenuItem::HIGHER, GuessHigher},
		{CombatMenuItem::LOWER, GuessLower},
		{CombatMenuItem::RUN_AWAY, RunAway},
		{CombatMenuItem::USE_ITEM, UseItem}
	};

	static void OnActivateItem()
	{
		common::Utility::Dispatch(activators, (CombatMenuItem)visuals::Menus::ReadValue(LAYOUT_NAME, COMBAT_MENU_ID).value());
	}

	static void NextItem()
	{
		if (visuals::Menus::ReadValue(LAYOUT_NAME, COMBAT_MENU_ID).value() == (int)CombatMenuItem::USE_ITEM) 
		{ 
			visuals::AvatarInventory::NextIndex(INVENTORY_LAYOUT_NAME, CONTROL_AVATAR_INVENTORY);
		}
	}

	static void PreviousItem()
	{
		if (visuals::Menus::ReadValue(LAYOUT_NAME, COMBAT_MENU_ID).value() == (int)CombatMenuItem::USE_ITEM) 
		{ 
			visuals::AvatarInventory::PreviousIndex(INVENTORY_LAYOUT_NAME, CONTROL_AVATAR_INVENTORY);
		}
	}

	static void LeavePlay()
	{
		application::UIState::Write(::UIState::LEAVE_PLAY);
	}

	std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, LeavePlay },
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, COMBAT_MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, COMBAT_MENU_ID) },
		{ ::Command::LEFT, PreviousItem },
		{ ::Command::RIGHT, NextItem },
		{ ::Command::GREEN, OnActivateItem },
	};

	static void UpdateUseItem()
	{
		auto item = visuals::AvatarInventory::GetItem(INVENTORY_LAYOUT_NAME, CONTROL_AVATAR_INVENTORY);
		std::stringstream ss;
		if (item)
		{
			auto descriptor = game::item::GetDescriptor(*item);
			if (descriptor.combatText)
			{
				ss << (*descriptor.combatText);
			}
		}
		else
		{
			ss << "(no items)";
		}
		visuals::MenuItems::SetText(LAYOUT_NAME, USE_ITEM_MENU_ITEM_ID, ss.str());
	}

	static void OnEnter()
	{
		visuals::Layouts::InitializeLayout(LAYOUT_NAME);
		auto& card = game::CombatDeck::GetCurrentCard();
		visuals::Images::SetSprite(LAYOUT_NAME, CURRENT_CARD_IMAGE_ID, visuals::CardSprites::GetSpriteForCard(card));
		UpdateUseItem();
	}

	const std::map<std::string, CombatMenuItem> areaMenuItems =
	{
		{ AREA_GUESS_HIGHER,  CombatMenuItem::HIGHER},
		{ AREA_GUESS_LOWER,  CombatMenuItem::LOWER},
		{ AREA_USE_ITEM,  CombatMenuItem::USE_ITEM},
		{ AREA_PREVIOUS_ITEM,  CombatMenuItem::USE_ITEM},
		{ AREA_NEXT_ITEM,  CombatMenuItem::USE_ITEM},
		{ AREA_RUN_AWAY,  CombatMenuItem::RUN_AWAY}
	};

	static void SetCurrentMenuItem(CombatMenuItem item)
	{
		visuals::Menus::WriteValue(LAYOUT_NAME, COMBAT_MENU_ID, (int)item);
	}

	static void OnMouseMotion(const common::XY<Sint32>& xy)
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		visuals::Texts::SetColor(LAYOUT_NAME, TEXT_PREVIOUS_ITEM, (areas.contains(AREA_PREVIOUS_ITEM)) ? (visuals::data::Colors::HIGHLIGHT) : (visuals::data::Colors::NORMAL));
		visuals::Texts::SetColor(LAYOUT_NAME, TEXT_NEXT_ITEM, (areas.contains(AREA_NEXT_ITEM)) ? (visuals::data::Colors::HIGHLIGHT) : (visuals::data::Colors::NORMAL));
		for (auto& area : areas)
		{
			SetCurrentMenuItem(areaMenuItems.find(area)->second);
		}
	}

	static bool ClickPreviousItem(const std::set<std::string>& areas)
	{
		if (areas.contains(AREA_PREVIOUS_ITEM))
		{
			PreviousItem();
			application::OnEnter::Handle();
			return true;
		}
		return false;
	}

	static bool ClickNextItem(const std::set<std::string>& areas)
	{
		if (areas.contains(AREA_NEXT_ITEM))
		{
			NextItem();
			application::OnEnter::Handle();
			return true;
		}
		return false;
	}

	static bool ClickAny(const std::set<std::string>& areas)
	{
		if (!areas.empty())
		{
			OnActivateItem();
			return true;
		}
		return false;
	}

	static bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8)//TODO: duplicated code with other menus
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		return ClickPreviousItem(areas) || ClickNextItem(areas) || ClickAny(areas);
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_COMBAT, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_COMBAT, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::IN_PLAY_COMBAT, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_COMBAT, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_COMBAT, OnEnter);
	}
}