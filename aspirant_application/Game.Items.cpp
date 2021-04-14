#include "Game.Items.h"
#include <map>
#include <vector>
#include "Game.Item.h"
#include "Common.XY.h"
#include "Game.World.h"
#include "Common.RNG.h"
#include "Data.JSON.h"
#include "Common.Properties.h"
#include <sstream>
namespace game::Items
{
	static nlohmann::json items;

	const std::map<game::Item, std::string> itemName =
	{
		{game::Item::FOOD, "Food"},
		{game::Item::POTION, "Potion"},
		{game::Item::BEER, "Beer"},
		{game::Item::WINE, "Wine"},
		{game::Item::COFFEE, "Coffee"},
		{game::Item::JOOLS, "Jools"}
	};

	std::string GetName(const game::Item& item)
	{
		std::stringstream ss;
		ss << (int)item;
		auto itemKey = ss.str();
		auto& itemRecord = items[itemKey];
		return itemRecord[common::Properties::NAME];
	}

	void InitializeFromFile(const std::string& filename)
	{
		items = data::JSON::Load(filename);
	}

}
