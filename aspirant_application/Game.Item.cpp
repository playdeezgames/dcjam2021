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
			descriptor[game::Properties::IMAGE_ID],
			(descriptor.count(game::Properties::NON_COMBAT)>0) ? (std::optional<Usage>((Usage)(int)descriptor[game::Properties::NON_COMBAT])) : (std::nullopt),
			(descriptor.count(game::Properties::AMOUNT) > 0) ? (std::optional<int>((int)descriptor[game::Properties::AMOUNT])) : (std::nullopt),
			(descriptor.count(game::Properties::DURATION) > 0) ? (std::optional<int>((int)descriptor[game::Properties::DURATION])) : (std::nullopt),
			(descriptor.count(game::Properties::SFX_SUCCESS) > 0) ? (std::optional<std::string>(descriptor[game::Properties::SFX_SUCCESS])) : (std::nullopt),
			(descriptor.count(game::Properties::SFX_FAILURE) > 0) ? (std::optional<std::string>(descriptor[game::Properties::SFX_FAILURE])) : (std::nullopt)
		};
	}

}