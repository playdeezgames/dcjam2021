#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Game.Combat.h"
#include "Graphics.Images.h"
#include "Graphics.Menus.h"
#include "Game.Avatar.h"
#include "Common.RNG.h"
#include "Common.Audio.h"
#include "Graphics.AvatarInventory.h"
#include <sstream>
#include "Graphics.MenuItems.h"
#include "Common.Audio.h"
#include "Application.Sounds.h"
#include "Game.Creatures.h"
#include "Game.Avatar.Items.h"
#include "Game.CombatDeck.h"
#include "Graphics.Texts.h"
#include "Graphics.CardSprites.h"
#include "Game.Avatar.Items.h"
#include "Game.Item.h"
namespace state::in_play::Combat
{
	const std::string LAYOUT_NAME = "State.InPlay.Combat";
	const std::string CURRENT_CARD_IMAGE_ID = "CurrentCard";
	const std::string COMBAT_MENU_ID = "Combat";
	const std::string USE_ITEM_MENU_ITEM_ID = "UseItem";

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

	void SetCombatResultText(const std::string& text)
	{
		graphics::Texts::SetText(COMBATRESULT_LAYOUT_NAME, COMBATRESULT_RESULT_TEXT_ID, text);
	}

	const std::map<game::Combat::CombatResult, std::tuple<std::string, std::string>> resolutions =
	{
		{game::Combat::CombatResult::MONSTER_KILLED, {COMBATRESULT_KILL_MONSTER, application::Sounds::DEAD_MONSTER}},
		{game::Combat::CombatResult::MONSTER_HIT, {COMBATRESULT_HIT_MONSTER, application::Sounds::HIT_MONSTER}},
		{game::Combat::CombatResult::MONSTER_BLOCKED, {COMBATRESULT_MISSED_MONSTER, application::Sounds::HIT_BLOCKED}},
		{game::Combat::CombatResult::HUNTER_HIT, {COMBATRESULT_GOT_HIT, application::Sounds::HIT_HUNTER}},
		{game::Combat::CombatResult::HUNTER_BLOCKED, {COMBATRESULT_BLOCKED_HIT, application::Sounds::HIT_BLOCKED}},
		{game::Combat::CombatResult::HUNTER_RAN, {COMBATRESULT_HUNTER_RAN, application::Sounds::HIT_HUNTER}}
	};

	void ResolveCombat(std::optional<game::CombatDeck::Guess> guess)
	{
		auto& resolutionDetails = resolutions.find(game::Combat::Resolve(guess))->second;
		SetCombatResultText(std::get<0>(resolutionDetails));
		common::audio::Sfx::Play(std::get<1>(resolutionDetails));
	}

	static void OnActivateItem()
	{
		switch ((CombatMenuItem)graphics::Menus::ReadValue(LAYOUT_NAME, COMBAT_MENU_ID).value())
		{
		case CombatMenuItem::HIGHER:
			ResolveCombat(game::CombatDeck::Guess::HIGHER);
			application::UIState::Write(::UIState::IN_PLAY_COMBAT_RESULT);
			break;
		case CombatMenuItem::LOWER:
			ResolveCombat(game::CombatDeck::Guess::LOWER);
			application::UIState::Write(::UIState::IN_PLAY_COMBAT_RESULT);
			break;
		case CombatMenuItem::RUN_AWAY:
			ResolveCombat(std::nullopt);
			Flee();
			break;
		case CombatMenuItem::USE_ITEM:
			auto result = game::avatar::Items::CombatUse(graphics::AvatarInventory::GetItem());
			if (result)
			{
				common::audio::Sfx::Play(std::get<0>(*result));
				if (std::get<1>(*result))
				{
					application::UIState::EnterGame();
				}
			}
			break;
		}
	}

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
			application::UIState::Write(::UIState::LEAVE_PLAY);
			break;
		case ::Command::UP:
			graphics::Menus::Previous(LAYOUT_NAME, COMBAT_MENU_ID);
			break;
		case ::Command::DOWN:
			graphics::Menus::Next(LAYOUT_NAME, COMBAT_MENU_ID);
			break;
		case ::Command::LEFT:
			if (graphics::Menus::ReadValue(LAYOUT_NAME, COMBAT_MENU_ID).value() == (int)CombatMenuItem::USE_ITEM)
			{
				graphics::AvatarInventory::PreviousIndex();
			}
			break;
		case ::Command::RIGHT:
			if (graphics::Menus::ReadValue(LAYOUT_NAME, COMBAT_MENU_ID).value() == (int)CombatMenuItem::USE_ITEM)
			{
				graphics::AvatarInventory::NextIndex();
			}
			break;
		case ::Command::GREEN:
			OnActivateItem();
			break;
		}
	}

	static void UpdateUseItem()
	{
		auto item = graphics::AvatarInventory::GetItem();
		std::stringstream ss;
		if (item)
		{
			auto descriptor = game::item::GetDescriptor(*item);
			if (descriptor.combatText)
			{
				ss << "< " << (*descriptor.combatText) << " >";
			}
		}
		else
		{
			ss << "(no items)";
		}
		graphics::MenuItems::SetText(LAYOUT_NAME, USE_ITEM_MENU_ITEM_ID, ss.str());
	}

	static void OnUpdate(const Uint32& ticks)
	{
		auto& card = game::CombatDeck::GetCurrentCard();
		graphics::Images::SetSprite(LAYOUT_NAME, CURRENT_CARD_IMAGE_ID, graphics::CardSprites::GetSpriteForCard(card));
		UpdateUseItem();
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_COMBAT, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_COMBAT, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT, OnUpdate);
	}
}