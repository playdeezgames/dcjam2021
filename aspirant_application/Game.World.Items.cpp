#include "Game.World.Items.h"
#include <map>
#include <vector>
#include "Common.RNG.h"
#include "Game.World.h"
namespace game::world::Items
{
	const std::map<game::Item, size_t> itemCounts =
	{
		{game::Item::FOOD, 72},
		{game::Item::POTION, 18},
		{game::Item::BEER, 9},
		{game::Item::WINE, 9},
		{game::Item::COFFEE, 9},
		{game::Item::JOOLS, 144}
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
		size_t totalCount = 0;
		for (auto itemCount : itemCounts)
		{
			auto item = itemCount.first;
			auto count = itemCount.second;
			while (count > 0)
			{
				totalCount++;
				PopulateItem(item);
				count--;
			}
		}
		totalCount = totalCount;
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
		auto iter = roomInventories[location.GetX()][location.GetY()].find(item);
		return iter != roomInventories[location.GetX()][location.GetY()].end();
	}

	const std::map<game::Item, size_t>& FloorInventory(const common::XY<size_t>& location)
	{
		return roomInventories[location.GetX()][location.GetY()];
	}

	size_t Remove(const game::Item& item, size_t quantity, const common::XY<size_t>& location)
	{
		if (IsPresent(item, location))
		{
			size_t total = roomInventories[location.GetX()][location.GetY()][item];
			if (quantity >= total)
			{
				quantity = total;
				roomInventories[location.GetX()][location.GetY()].erase(item);
			}
			else
			{
				roomInventories[location.GetX()][location.GetY()][item] -= quantity;
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
			roomInventories[location.GetX()][location.GetY()][item] += amount;
		}
	}
}