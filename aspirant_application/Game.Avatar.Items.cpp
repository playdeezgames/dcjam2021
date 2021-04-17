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
		for (auto item : game::item::All())
		{
			auto descriptor = game::item::GetDescriptor(item);
			if (descriptor.initialInventory)
			{
				Add(item, descriptor.initialInventory.value());
			}
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

	static size_t Remove(game::Item item, size_t quantity)
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

	static std::optional<std::string> ConsumeItem(game::Item item, std::function<void(const game::item::Descriptor&)> action)
	{
		//TODO: check if verb is valid? but only if it becomes non-static
		auto descriptor = game::item::GetDescriptor(item);
		if (game::avatar::Items::Read(item) > 0)
		{
			action(descriptor);
			game::avatar::Items::Remove(item, 1);
			return descriptor.sfxSuccess;
		}
		return descriptor.sfxFailure;
	}

	static std::optional<std::string> Eat(game::Item item)
	{
		return ConsumeItem(item, [](const game::item::Descriptor& descriptor) 
		{
			game::avatar::Statistics::Increase(game::avatar::Statistic::HUNGER, descriptor.amount.value());
		});
	}

	std::optional<std::string> Heal(game::Item item)
	{
		return ConsumeItem(item, [](const game::item::Descriptor& descriptor)
		{
			game::avatar::Statistics::Increase(game::avatar::Statistic::HEALTH, descriptor.amount.value());
		});
	}

	std::optional<std::string> BuffAttack(game::Item item)
	{
		return ConsumeItem(item, [](const game::item::Descriptor& descriptor)
		{
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK, descriptor.amount.value());
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK_TIMER, descriptor.duration.value());
		});
	}

	std::optional<std::string> BuffDefend(game::Item item)
	{
		return ConsumeItem(item, [](const game::item::Descriptor& descriptor)
		{
			game::avatar::Statistics::Write(game::avatar::Statistic::DEFEND, descriptor.amount.value());
			game::avatar::Statistics::Write(game::avatar::Statistic::DEFEND_TIMER, descriptor.duration.value());
		});
	}

	static std::map<game::item::Usage, std::function<std::optional<std::string>(game::Item)>> nonCombatVerbs =
	{
		{game::item::Usage::EAT, Eat},
		{game::item::Usage::HEAL, Heal},
		{game::item::Usage::ATTACK_BUFF, BuffAttack},
		{game::item::Usage::DEFEND_BUFF, BuffDefend},
		{game::item::Usage::BRIBE, [](game::Item) { return std::nullopt; }}
	};

	std::optional<std::string> Use(std::optional<game::Item> item)
	{
		if (item)
		{
			auto usage = game::item::GetDescriptor(*item).usage;
			if (usage)
			{
				return nonCombatVerbs.find(*usage)->second(*item);
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