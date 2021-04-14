#include "Game.Avatar.Items.h"
#include <map>
#include "json.hpp"
#include "Game.Properties.h"
#include <sstream>
#include "Common.Utility.h"
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
}