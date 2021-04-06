#include "Game.Avatar.Items.h"
#include <map>
namespace game::avatar::Items
{
	static std::map<game::Item, size_t> avatarInventory;
	const std::map<game::Item, size_t> initial =
	{
		{game::Item::FOOD, 10},
		{game::Item::POTION, 3}
	};

	size_t Read(game::Item item)
	{
		auto iter = avatarInventory.find(item);
		if (iter != avatarInventory.end())
		{
			return iter->second;
		}
		else
		{
			return 0;
		}
	}

	const std::map<game::Item, size_t>& All()
	{
		return avatarInventory;
	}

	void Reset()
	{
		avatarInventory = initial;
	}

	void Add(game::Item item, size_t amount)
	{
		if (amount > 0)
		{
			avatarInventory[item] += amount;
		}
	}

	void Start()
	{
		Reset();
	}

	size_t Remove(game::Item item, size_t quantity)
	{
		if (avatarInventory.find(item)!=avatarInventory.end())
		{
			size_t total = avatarInventory[item];
			if (quantity >= total)
			{
				quantity = total;
				avatarInventory.erase(item);
			}
			else
			{
				avatarInventory[item] -= quantity;
			}
			return quantity;
		}
		else
		{
			return 0;
		}

	}
}