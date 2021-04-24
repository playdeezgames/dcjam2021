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
#include "Common.Utility.h"
#include "Game.Creatures.h"
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

	const std::map<game::Combat::CombatResult, std::string> resolutions =
	{
		{game::Combat::CombatResult::MONSTER_KILLED, COMBATRESULT_KILL_MONSTER},
		{game::Combat::CombatResult::MONSTER_HIT, COMBATRESULT_HIT_MONSTER},
		{game::Combat::CombatResult::MONSTER_BLOCKED, COMBATRESULT_MISSED_MONSTER},
		{game::Combat::CombatResult::HUNTER_HIT, COMBATRESULT_GOT_HIT},
		{game::Combat::CombatResult::HUNTER_BLOCKED, COMBATRESULT_BLOCKED_HIT},
		{game::Combat::CombatResult::HUNTER_RAN, COMBATRESULT_HUNTER_RAN}
	};

	const std::map<game::Combat::CombatResult, std::function<std::optional<std::string>(const std::optional<game::Creatures::Instance>&)>> sounds =
	{
		{game::Combat::CombatResult::MONSTER_KILLED, [](const std::optional<game::Creatures::Instance>& instance) { return instance.value().descriptor.sfx.find(game::creature::Sfx::DEATH)->second; }},
		{game::Combat::CombatResult::MONSTER_HIT, [](const std::optional<game::Creatures::Instance>& instance) { return instance.value().descriptor.sfx.find(game::creature::Sfx::HIT)->second; }},
		{game::Combat::CombatResult::MONSTER_BLOCKED, [](const std::optional<game::Creatures::Instance>& instance) { return instance.value().descriptor.sfx.find(game::creature::Sfx::BLOCK)->second; }},
		{game::Combat::CombatResult::HUNTER_HIT, [](const std::optional<game::Creatures::Instance>&) { return application::Sounds::Read(application::UI::Sfx::HIT_HUNTER); }},
		{game::Combat::CombatResult::HUNTER_BLOCKED, [](const std::optional<game::Creatures::Instance>&) { return application::Sounds::Read(application::UI::Sfx::HIT_BLOCKED); }},
		{game::Combat::CombatResult::HUNTER_RAN, [](const std::optional<game::Creatures::Instance>&) { return application::Sounds::Read(application::UI::Sfx::HIT_HUNTER); }}
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
		auto result = game::avatar::Items::CombatUse(graphics::AvatarInventory::GetItem());
		if (result)
		{
			common::audio::Sfx::Play(std::get<0>(*result));
			if (std::get<1>(*result))
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
		common::Utility::Dispatch(activators, (CombatMenuItem)graphics::Menus::ReadValue(LAYOUT_NAME, COMBAT_MENU_ID).value());
	}

	std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, []() { application::UIState::Write(::UIState::LEAVE_PLAY); }},
		{ ::Command::UP, graphics::Menus::NavigatePrevious(LAYOUT_NAME, COMBAT_MENU_ID) },
		{ ::Command::DOWN, graphics::Menus::NavigateNext(LAYOUT_NAME, COMBAT_MENU_ID) },
		{ ::Command::LEFT, []() { if (graphics::Menus::ReadValue(LAYOUT_NAME, COMBAT_MENU_ID).value() == (int)CombatMenuItem::USE_ITEM) { graphics::AvatarInventory::PreviousIndex(); } } },
		{ ::Command::RIGHT, []() { if (graphics::Menus::ReadValue(LAYOUT_NAME, COMBAT_MENU_ID).value() == (int)CombatMenuItem::USE_ITEM) { graphics::AvatarInventory::NextIndex(); } } },
		{ ::Command::GREEN, OnActivateItem },
	};

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
		::application::Command::SetHandlers(::UIState::IN_PLAY_COMBAT, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_COMBAT, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT, OnUpdate);
	}
}