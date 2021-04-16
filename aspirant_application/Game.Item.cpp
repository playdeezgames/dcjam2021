#include "Game.Item.h"
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
namespace game::item
{
	static nlohmann::json items;

	void InitializeFromFile(const std::string& filename)
	{
		items = data::JSON::Load(filename);
	}

	static std::string ItemToItemKey(const game::Item& item)
	{
		std::stringstream ss;
		ss << (int)item;
		return ss.str();
	}

	Descriptor GetDescriptor(game::Item item)
	{
		auto descriptor = items[ItemToItemKey(item)];
		return
		{
			descriptor[common::Properties::NAME],
			descriptor[game::Properties::IMAGE_ID]
		};
	}

}