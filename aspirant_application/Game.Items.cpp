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
		{game::Item::POTION, "Potion"},
		{game::Item::BEER, "Beer"},
		{game::Item::WINE, "Wine"},
		{game::Item::COFFEE, "Coffee"},
		{game::Item::JOOLS, "Jools"}
	};

	const std::string& GetName(const game::Item& item)
	{
		return itemName.find(item)->second;
	}
}
