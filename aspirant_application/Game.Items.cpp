#include "Game.Items.h"
#include <map>
#include <vector>
#include "Game.Item.h"
#include "Common.XY.h"
#include "Game.World.h"
#include "Common.RNG.h"
#include "Data.JSON.h"
#include "Common.Properties.h"
#include "Game.Properties.h"
#include <sstream>
namespace game::Items
{
	static nlohmann::json items;

	static std::string ItemToItemKey(const game::Item& item)
	{
		std::stringstream ss;
		ss << (int)item;
		return ss.str();
	}

	std::string GetName(const game::Item& item)
	{
		return items[ItemToItemKey(item)][common::Properties::NAME];
	}

	std::string GetImageId(const game::Item& item)
	{
		return items[ItemToItemKey(item)][game::Properties::IMAGE_ID];
	}


	void InitializeFromFile(const std::string& filename)
	{
		items = data::JSON::Load(filename);
	}

}
