#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.AvatarInventory.h"
#include "Game.Avatar.Items.h"
#include "Game.Items.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Statistics.h"
namespace state::in_play::AvatarInventory
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarInventory";
	const int FOOD_HUNGER_INCREASE = 10;
	const int POTION_HEALTH_INCREASE = 50;

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
			game::avatar::Statistics::Increase(game::avatar::Statistic::HEALTH, POTION_HEALTH_INCREASE);
			game::avatar::Items::Remove(game::Item::POTION, 1);
		}
	}

	static void UseItem()
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