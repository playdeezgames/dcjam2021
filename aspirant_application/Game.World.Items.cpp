#include "Game.World.Items.h"
#include <map>
#include <vector>
#include "Common.RNG.h"
#include "Game.World.h"
namespace game::world::Items
{
	const std::map<int, size_t> itemCounts =
	{
		{0, 72},
		{1, 18},
		{2, 9},
		{3, 9},
		{4, 9},
		{5, 144}
	};

	static std::vector<std::vector<std::map<int, size_t>>> roomInventories;

	static void PopulateItem(int item)
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
			roomInventories.push_back(std::vector<std::map<int, size_t>>());
			auto& column = roomInventories.back();
			while (column.size() < worldSize.GetY())
			{
				column.push_back({});
			}
		}
		PopulateItems();
	}

	bool IsPresent(const common::XY<size_t>& location, const int& item)
	{
		auto iter = roomInventories[location.GetX()][location.GetY()].find(item);
		return iter != roomInventories[location.GetX()][location.GetY()].end();
	}

	const std::map<int, size_t>& FloorInventory(const common::XY<size_t>& location)
	{
		return roomInventories[location.GetX()][location.GetY()];
	}

	size_t Remove(const common::XY<size_t>& location, const int& item, size_t quantity)
	{
		if (IsPresent(location, item))
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

	void Add(const common::XY<size_t>& location, const int& item, size_t amount)
	{
		if (amount > 0)
		{
			roomInventories[location.GetX()][location.GetY()][item] += amount;
		}
	}
}