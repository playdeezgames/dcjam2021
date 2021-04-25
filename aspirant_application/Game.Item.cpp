#include "Game.Item.h"
#include <map>
#include <vector>
#include "Game.Item.h"
#include "Common.XY.h"
#include "Game.World.h"
#include "Common.RNG.h"
#include "Data.JSON.h"
#include "Common.Data.Properties.h"
#include "Game.Data.Properties.h"
#include <sstream>
#include "Common.Utility.h"
#include "Data.Stores.h"
namespace game::item
{
	static std::string ItemToItemKey(const int& item)
	{
		std::stringstream ss;
		ss << (int)item;
		return ss.str();
	}

	std::vector<int> All()
	{
		std::vector<int> result;
		for (auto& item : ::data::Stores::GetStore(::data::Store::ITEM_DESCRIPTORS).items())
		{
			result.push_back(common::Utility::StringToInt(item.key()));
		}
		return result;
	}

	Descriptor GetDescriptor(int item)
	{
		auto& descriptor = ::data::Stores::GetStore(::data::Store::ITEM_DESCRIPTORS)[ItemToItemKey(item)];
		return
		{
			descriptor[common::data::Properties::NAME],
			descriptor[game::data::Properties::IMAGE_ID],
			descriptor[game::data::Properties::TAKE_IMAGE_ID],
			descriptor[game::data::Properties::TAKE_AREA_ID],
			(descriptor.count(game::data::Properties::NON_COMBAT)>0) ? (std::optional<Usage>((Usage)(int)descriptor[game::data::Properties::NON_COMBAT])) : (std::nullopt),
			(descriptor.count(game::data::Properties::AMOUNT) > 0) ? (std::optional<int>((int)descriptor[game::data::Properties::AMOUNT])) : (std::nullopt),
			(descriptor.count(game::data::Properties::DURATION) > 0) ? (std::optional<int>((int)descriptor[game::data::Properties::DURATION])) : (std::nullopt),
			(descriptor.count(game::data::Properties::SFX_SUCCESS) > 0) ? (std::optional<std::string>(descriptor[game::data::Properties::SFX_SUCCESS])) : (std::nullopt),
			(descriptor.count(game::data::Properties::SFX_FAILURE) > 0) ? (std::optional<std::string>(descriptor[game::data::Properties::SFX_FAILURE])) : (std::nullopt),
			(descriptor.count(game::data::Properties::INITIAL_INVENTORY) > 0) ? (std::optional<size_t>((size_t)descriptor[game::data::Properties::INITIAL_INVENTORY])) : (std::nullopt),
			(descriptor.count(game::data::Properties::COMBAT) > 0) ? (std::optional<Usage>((Usage)(int)descriptor[game::data::Properties::COMBAT])) : (std::nullopt),
			(descriptor.count(game::data::Properties::NUMBER_APPEARING) > 0) ? ((size_t)descriptor[game::data::Properties::NUMBER_APPEARING]) : (0),
			(descriptor.count(game::data::Properties::COMBAT_TEXT) > 0) ? (std::optional<std::string>(descriptor[game::data::Properties::COMBAT_TEXT])) : (std::nullopt)
		};
	}

}