#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Game.Combat.h"
#include "Graphics.Images.h"
#include "Graphics.Menus.h"
#include "Game.Avatar.h"
#include "Common.RNG.h"
#include "Common.Sounds.h"
#include "Graphics.AvatarInventory.h"
#include <sstream>
#include "Graphics.MenuItems.h"
#include "Common.Sounds.h"
#include "Application.Sounds.h"
#include "Game.Creatures.h"
#include "Game.Avatar.Items.h"
namespace state::in_play::AvatarInventory
{
	void UseItem();
}
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
		RUN_AWAY,
		USE_ITEM
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

	static void MoneyBribe()
	{
		auto amount = game::Creatures::GetMoneyBribe(game::Avatar::GetPosition()).value_or(0);
		if (amount > 0 && game::avatar::Items::Read(game::Item::JOOLS) >= amount)
		{
			game::avatar::Items::Remove(game::Item::JOOLS, (size_t)amount);
			common::Sounds::PlaySound(application::Sounds::WOOHOO);
			game::Creatures::Remove(game::Avatar::GetPosition());
			application::UIState::EnterGame();
		}
		else
		{
			common::Sounds::PlaySound(application::Sounds::SHUCKS);
		}
	}

	static void FoodBribe()
	{
		auto amount = game::Creatures::GetFoodBribe(game::Avatar::GetPosition()).value_or(0);
		if (amount > 0 && game::avatar::Items::Read(game::Item::FOOD) >= amount)
		{
			game::avatar::Items::Remove(game::Item::FOOD, (size_t)amount);
			common::Sounds::PlaySound(application::Sounds::WOOHOO);
			game::Creatures::Remove(game::Avatar::GetPosition());
			application::UIState::EnterGame();
		}
		else
		{
			common::Sounds::PlaySound(application::Sounds::SHUCKS);
		}
	}

	static void UseItem()
	{
		auto item = graphics::AvatarInventory::GetItem();
		if (item)
		{
			switch (item.value())
			{
			case game::Item::BEER:
			case game::Item::WINE:
			case game::Item::COFFEE:
			case game::Item::POTION:
				state::in_play::AvatarInventory::UseItem();
				break;
			case game::Item::JOOLS:
				MoneyBribe();
				break;
			case game::Item::FOOD:
				FoodBribe();
				break;
			}
		}
		else
		{
			common::Sounds::PlaySound(application::Sounds::SHUCKS);
		}
	}

	static void OnActivateItem()
	{
		switch ((CombatMenuItem)graphics::Menus::Read(LAYOUT_NAME, COMBAT_MENU_ID).value())
		{
		case CombatMenuItem::HIGHER:
			game::Combat::Resolve(game::Combat::Guess::HIGHER);
			application::UIState::Write(::UIState::IN_PLAY_COMBAT_RESULT);
			break;
		case CombatMenuItem::LOWER:
			game::Combat::Resolve(game::Combat::Guess::LOWER);
			application::UIState::Write(::UIState::IN_PLAY_COMBAT_RESULT);
			break;
		case CombatMenuItem::RUN_AWAY:
			game::Combat::Resolve(std::nullopt);
			Flee();
			break;
		case CombatMenuItem::USE_ITEM:
			UseItem();
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
			if (graphics::Menus::Read(LAYOUT_NAME, COMBAT_MENU_ID).value() == (int)CombatMenuItem::USE_ITEM)
			{
				graphics::AvatarInventory::PreviousIndex();
			}
			break;
		case ::Command::RIGHT:
			if (graphics::Menus::Read(LAYOUT_NAME, COMBAT_MENU_ID).value() == (int)CombatMenuItem::USE_ITEM)
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
			ss << "< ";
			switch (item.value())
			{
			case game::Item::BEER:
				ss << "Drink Beer";
				break;
			case game::Item::COFFEE:
				ss << "Drink Coffee";
				break;
			case game::Item::FOOD:
				ss << "Food Bribe";
				break;
			case game::Item::JOOLS:
				ss << "Money Bribe";
				break;
			case game::Item::POTION:
				ss << "Drink Potion";
				break;
			case game::Item::WINE:
				ss << "Drink Wine";
				break;
			}
			ss << " >";
		}
		else
		{
			ss << "(no items)";
		}
		graphics::MenuItems::SetItemText(LAYOUT_NAME, USE_ITEM_MENU_ITEM_ID, ss.str());
	}

	static void OnUpdate(const Uint32& ticks)
	{
		auto& card = game::CombatDeck::GetCurrentCard();
		graphics::Images::SetSprite(LAYOUT_NAME, CURRENT_CARD_IMAGE_ID, game::CombatDeck::GetSpriteForCard(card));
		UpdateUseItem();
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_COMBAT, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_COMBAT, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT, OnUpdate);
	}
}