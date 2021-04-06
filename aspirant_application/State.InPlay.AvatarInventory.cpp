#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.AvatarInventory.h"
#include "Game.Avatar.Items.h"
#include "Game.Items.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Statistics.h"
#include "Common.Sounds.h"
#include "Application.Sounds.h"
namespace state::in_play::AvatarInventory
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarInventory";
	const int FOOD_HUNGER_INCREASE = 10;
	const int POTION_HEALTH_INCREASE = 50;
	const int WINE_ATTACK = 25;
	const int WINE_ATTACK_DURATION = 50;
	const int BEER_ATTACK = 50;
	const int BEER_ATTACK_DURATION = 25;
	const int COFFEE_DEFEND_INCREASE = 10;
	const int COFFEE_DEFEND_DURATION = 25;

	static void DropItem()
	{
		auto item = graphics::AvatarInventory::GetItem();
		if (item)
		{
			size_t amount = game::avatar::Items::Remove(*item, 1);
			game::Items::Add(*item, 1, game::Avatar::GetPosition());
		}
	}

	static void EatFood()
	{
		if (game::avatar::Items::Read(game::Item::FOOD)>0)
		{
			game::avatar::Statistics::Increase(game::avatar::Statistic::HUNGER, FOOD_HUNGER_INCREASE);
			game::avatar::Items::Remove(game::Item::FOOD, 1);
		}
	}

	static void DrinkPotion()
	{
		if (game::avatar::Items::Read(game::Item::POTION)>0)
		{
			common::Sounds::PlaySound(application::Sounds::DRINK_POTION);
			game::avatar::Statistics::Increase(game::avatar::Statistic::HEALTH, POTION_HEALTH_INCREASE);
			game::avatar::Items::Remove(game::Item::POTION, 1);
		}
	}

	static void DrinkBeer()
	{
		if (game::avatar::Items::Read(game::Item::BEER) > 0)
		{
			common::Sounds::PlaySound(application::Sounds::BEER);
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK, BEER_ATTACK);
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK_TIMER, BEER_ATTACK_DURATION);
			game::avatar::Items::Remove(game::Item::BEER, 1);
		}
	}

	static void DrinkWine()
	{
		if (game::avatar::Items::Read(game::Item::WINE) > 0)
		{
			common::Sounds::PlaySound(application::Sounds::WINE);
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK, WINE_ATTACK);
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK_TIMER, WINE_ATTACK_DURATION);
			game::avatar::Items::Remove(game::Item::WINE, 1);
		}
	}

	static void DrinkCoffee()
	{
		if (game::avatar::Items::Read(game::Item::COFFEE) > 0)
		{
			common::Sounds::PlaySound(application::Sounds::COFFEE);
			game::avatar::Statistics::Increase(game::avatar::Statistic::DEFEND, COFFEE_DEFEND_INCREASE);
			game::avatar::Statistics::Write(game::avatar::Statistic::DEFEND_TIMER, COFFEE_DEFEND_DURATION);
			game::avatar::Items::Remove(game::Item::COFFEE, 1);
		}
	}

	void UseItem()
	{
		auto item = graphics::AvatarInventory::GetItem();
		if (item)
		{
			switch (*item)
			{
			case game::Item::FOOD:
				EatFood();
				break;
			case game::Item::POTION:
				DrinkPotion();
				break;
			case game::Item::BEER:
				DrinkBeer();
				break;
			case game::Item::WINE:
				DrinkWine();
				break;
			case game::Item::COFFEE:
				DrinkCoffee();
				break;
			}
		}
	}

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
			application::UIState::Write(::UIState::LEAVE_PLAY);
			break;
		case ::Command::PREVIOUS:
			graphics::AvatarInventory::ResetIndex();
			application::UIState::Write(::UIState::IN_PLAY_FLOOR);
			break;
		case ::Command::NEXT:
		case ::Command::YELLOW:
			graphics::AvatarInventory::ResetIndex();
			application::UIState::Write(::UIState::IN_PLAY_STATUS);
			break;
		case ::Command::UP:
			graphics::AvatarInventory::PreviousIndex();
			break;
		case ::Command::DOWN:
			graphics::AvatarInventory::NextIndex();
			break;
		case ::Command::RED:
			DropItem();
			break;
		case ::Command::GREEN:
			UseItem();
			break;
		}
	}

	static void OnUpdate(const Uint32& ticks)
	{

	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_INVENTORY, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_INVENTORY, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_INVENTORY, OnUpdate);
	}
}