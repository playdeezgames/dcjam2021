#include "Game.Avatar.Items.h"
#include <map>
#include "json.hpp"
#include "Game.Properties.h"
#include <sstream>
#include "Common.Utility.h"
#include "Game.World.Items.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Avatar.h"
#include "Application.Sounds.h"
#include "Game.Creatures.h"
namespace game::Avatar
{
	nlohmann::json& GetAvatar();
}
namespace game::avatar::Items
{
	nlohmann::json& GetAvatarInventory()
	{
		auto& avatar = game::Avatar::GetAvatar();
		if (avatar.count(game::Properties::INVENTORY) == 0)
		{
			avatar[game::Properties::INVENTORY] = nlohmann::json();
		}
		return avatar[game::Properties::INVENTORY];
	}

	const std::map<game::Item, size_t> initial =
	{
		{game::Item::FOOD, 5},
		{game::Item::POTION, 2}
	};

	static std::string ItemToKey(game::Item item)
	{
		std::stringstream ss;
		ss << (int)item;
		return ss.str();
	}

	size_t Read(game::Item item)
	{
		auto itemKey = ItemToKey(item);
		auto& inventory = GetAvatarInventory();
		if (inventory.count(itemKey) > 0)
		{
			return inventory[itemKey];
		}
		return 0;
	}

	std::map<game::Item, size_t> All()
	{
		std::map<game::Item, size_t> result;
		for (auto& item : GetAvatarInventory().items())
		{
			result[(game::Item)common::Utility::StringToInt(item.key())] = item.value();
		}
		return result;
	}

	void Reset()
	{
		GetAvatarInventory().clear();
		for (auto& item : initial)
		{
			Add(item.first, item.second);
		}
	}

	void Add(game::Item item, size_t amount)
	{
		if (amount > 0)
		{
			auto itemKey = ItemToKey(item);
			GetAvatarInventory()[itemKey] = Read(item) + amount;
		}
	}

	void Start()
	{
		Reset();
	}

	size_t Remove(game::Item item, size_t quantity)
	{
		auto total = Read(item);
		if (total >0)
		{
			auto itemKey = ItemToKey(item);
			if (quantity >= total)
			{
				quantity = total;
				GetAvatarInventory().erase(itemKey);
			}
			else
			{
				GetAvatarInventory()[itemKey] = Read(item) -quantity;
			}
			return quantity;
		}
		else
		{
			return 0;
		}

	}

	const int FOOD_HUNGER_INCREASE = 5;
	const int POTION_HEALTH_INCREASE = 30;
	const int WINE_ATTACK = 25;
	const int WINE_ATTACK_DURATION = 25;
	const int BEER_ATTACK = 50;
	const int BEER_ATTACK_DURATION = 10;
	const int COFFEE_DEFEND_INCREASE = 10;
	const int COFFEE_DEFEND_DURATION = 10;

	void Drop(std::optional<game::Item> item)
	{
		if (item)
		{
			size_t amount = game::avatar::Items::Remove(*item, 1);
			game::world::Items::Add(*item, 1, game::Avatar::GetPosition());
		}
	}

	static std::optional<std::string> EatFood()
	{
		if (game::avatar::Items::Read(game::Item::FOOD) > 0)
		{
			game::avatar::Statistics::Increase(game::avatar::Statistic::HUNGER, FOOD_HUNGER_INCREASE);
			game::avatar::Items::Remove(game::Item::FOOD, 1);
		}
		return std::nullopt;//TODO: make sound associated with eating
	}

	static std::optional<std::string> DrinkPotion()
	{
		if (game::avatar::Items::Read(game::Item::POTION) > 0)
		{
			game::avatar::Statistics::Increase(game::avatar::Statistic::HEALTH, POTION_HEALTH_INCREASE);
			game::avatar::Items::Remove(game::Item::POTION, 1);
			return application::Sounds::DRINK_POTION;
		}
		return std::nullopt;
	}

	static std::optional<std::string> DrinkBeer()
	{
		if (game::avatar::Items::Read(game::Item::BEER) > 0)
		{
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK, BEER_ATTACK);
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK_TIMER, BEER_ATTACK_DURATION);
			game::avatar::Items::Remove(game::Item::BEER, 1);
			return application::Sounds::BEER;
		}
		return std::nullopt;
	}

	static std::optional<std::string> DrinkWine()
	{
		if (game::avatar::Items::Read(game::Item::WINE) > 0)
		{
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK, WINE_ATTACK);
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK_TIMER, WINE_ATTACK_DURATION);
			game::avatar::Items::Remove(game::Item::WINE, 1);
			return application::Sounds::WINE;
		}
		return std::nullopt;
	}

	static std::optional<std::string> DrinkCoffee()
	{
		if (game::avatar::Items::Read(game::Item::COFFEE) > 0)
		{
			game::avatar::Statistics::Increase(game::avatar::Statistic::DEFEND, COFFEE_DEFEND_INCREASE);
			game::avatar::Statistics::Write(game::avatar::Statistic::DEFEND_TIMER, COFFEE_DEFEND_DURATION);
			game::avatar::Items::Remove(game::Item::COFFEE, 1);
			return application::Sounds::COFFEE;
		}
		return std::nullopt;
	}

	std::optional<std::string> Use(std::optional<game::Item> item)
	{
		if (item)
		{
			switch (*item)
			{
			case game::Item::FOOD:
				return EatFood();
			case game::Item::POTION:
				return DrinkPotion();
			case game::Item::BEER:
				return DrinkBeer();
			case game::Item::WINE:
				return DrinkWine();
			case game::Item::COFFEE:
				return DrinkCoffee();
			}
		}
		return std::nullopt;
	}

	static std::optional<std::tuple<std::string, bool>> MoneyBribe()
	{
		auto instance = game::Creatures::GetInstance(game::Avatar::GetPosition());
		size_t amount = (instance.has_value()) ? (instance.value().descriptor.moneyBribe) : (0);
		if (amount > 0 && game::avatar::Items::Read(game::Item::JOOLS) >= amount)
		{
			game::avatar::Items::Remove(game::Item::JOOLS, (size_t)amount);
			game::Creatures::Remove(game::Avatar::GetPosition());
			return std::make_tuple(application::Sounds::WOOHOO, true);
		}
		return std::make_tuple(application::Sounds::SHUCKS, false);
	}

	static std::optional<std::tuple<std::string, bool>> FoodBribe()
	{
		auto instance = game::Creatures::GetInstance(game::Avatar::GetPosition());
		size_t amount = (instance.has_value()) ? (instance.value().descriptor.foodBribe) : (0);
		if (amount > 0 && game::avatar::Items::Read(game::Item::FOOD) >= amount)
		{
			game::avatar::Items::Remove(game::Item::FOOD, (size_t)amount);
			game::Creatures::Remove(game::Avatar::GetPosition());
			return std::make_tuple(application::Sounds::WOOHOO, true);

		}
		return std::make_tuple(application::Sounds::SHUCKS, false);
	}

	std::optional<std::tuple<std::string, bool>> CombatUse(std::optional<game::Item> item)
	{
		if (item)
		{
			switch (item.value())
			{
			case game::Item::BEER:
			case game::Item::WINE:
			case game::Item::COFFEE:
			case game::Item::POTION:
			{
				auto useResult = game::avatar::Items::Use(item);
				if (useResult)
				{
					return std::make_tuple(*useResult, false);
				}
			}
			break;
			case game::Item::JOOLS:
				return MoneyBribe();
			case game::Item::FOOD:
				return FoodBribe();
			}
		}
		return std::make_tuple(application::Sounds::SHUCKS, false);
	}
}