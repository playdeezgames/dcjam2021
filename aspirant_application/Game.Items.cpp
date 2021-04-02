#include "Game.Items.h"
#include <map>
#include <vector>
#include "Game.Item.h"
#include "Common.XY.h"
#include "Game.World.h"
#include "Common.RNG.h"
namespace game::Items
{
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
		return roomInventories[location.GetX()][location.GetX()][item] > 0;
	}
}