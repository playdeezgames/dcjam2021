#include "Game.Items.h"
#include <map>
#include <vector>
#include "Game.Item.h"
#include "Common.XY.h"
#include "Game.World.h"
#include "Common.RNG.h"
namespace game::Items
{
	const std::map<game::Item, std::string> itemName =
	{
		{game::Item::FOOD, "Food"},
		{game::Item::POTION, "Potion"}
	};

	const std::string& GetName(const game::Item& item)
	{
		return itemName.find(item)->second;
	}

	const std::map<game::Item, size_t> itemCounts =
	{
		{game::Item::FOOD, 100},
		{game::Item::POTION, 50}
	};

	static std::vector<std::vector<std::map<game::Item, size_t>>> roomInventories;

	static void PopulateItem(game::Item item)
	{
		size_t column = (size_t)common::RNG::FromRange(0, (int)roomInventories.size());
		size_t row = (size_t)common::RNG::FromRange(0, (int)roomInventories[column].size());
		roomInventories[column][row][item]++;
	}

	static void PopulateItems()
	{
		for (auto itemCount : itemCounts)
		{
			auto item = itemCount.first;
			auto count = itemCount.second;
			while (count > 0)
			{
				PopulateItem(item);
				count--;
			}
		}
	}

	void Reset()
	{
		roomInventories.clear();
		auto worldSize = game::World::GetSize();
		while (roomInventories.size() < worldSize.GetX())
		{
			roomInventories.push_back(std::vector<std::map<game::Item, size_t>>());
			auto& column = roomInventories.back();
			while (column.size() < worldSize.GetY())
			{
				column.push_back({});
			}
		}
		PopulateItems();
	}

	bool IsPresent(const game::Item& item, const common::XY<size_t>& location)
	{
		auto iter = roomInventories[location.GetX()][location.GetX()].find(item);
		return iter != roomInventories[location.GetX()][location.GetX()].end();
	}

	const std::map<game::Item, size_t>& FloorInventory(const common::XY<size_t>& location)
	{
		return roomInventories[location.GetX()][location.GetX()];
	}

	size_t Remove(const game::Item& item, size_t quantity, const common::XY<size_t>& location)
	{
		if (IsPresent(item, location))
		{
			size_t total = roomInventories[location.GetX()][location.GetX()][item];
			if (quantity >= total)
			{
				quantity = total;
				roomInventories[location.GetX()][location.GetX()].erase(item);
			}
			else
			{
				roomInventories[location.GetX()][location.GetX()][item] -= quantity;
			}
			return quantity;
		}
		else
		{
			return 0;
		}
	}

	void Add(const game::Item& item, size_t amount, const common::XY<size_t>& location)
	{
		if (amount > 0)
		{
			roomInventories[location.GetX()][location.GetX()][item] += amount;
		}
	}
}