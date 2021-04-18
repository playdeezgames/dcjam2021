#include "Game.Item.h"
#include <map>
#include <vector>
#include "Game.Item.h"
#include "Common.XY.h"
#include "Game.World.h"
#include "Common.RNG.h"
#include "Data.JSON.h"
#include "Common.Data.Properties.h"
#include "Game.Properties.h"
#include <sstream>
#include "Common.Utility.h"
namespace game::item
{
	static nlohmann::json items;

	void InitializeFromFile(const std::string& filename)
	{
		items = data::JSON::Load(filename);
	}

	static std::string ItemToItemKey(const int& item)
	{
		std::stringstream ss;
		ss << (int)item;
		return ss.str();
	}

	std::vector<int> All()
	{
		std::vector<int> result;
		for (auto& item : items.items())
		{
			result.push_back(common::Utility::StringToInt(item.key()));
		}
		return result;
	}

	Descriptor GetDescriptor(int item)
	{
		auto descriptor = items[ItemToItemKey(item)];
		return
		{
			descriptor[common::data::Properties::NAME],
			descriptor[game::Properties::IMAGE_ID],
			(descriptor.count(game::Properties::NON_COMBAT)>0) ? (std::optional<Usage>((Usage)(int)descriptor[game::Properties::NON_COMBAT])) : (std::nullopt),
			(descriptor.count(game::Properties::AMOUNT) > 0) ? (std::optional<int>((int)descriptor[game::Properties::AMOUNT])) : (std::nullopt),
			(descriptor.count(game::Properties::DURATION) > 0) ? (std::optional<int>((int)descriptor[game::Properties::DURATION])) : (std::nullopt),
			(descriptor.count(game::Properties::SFX_SUCCESS) > 0) ? (std::optional<std::string>(descriptor[game::Properties::SFX_SUCCESS])) : (std::nullopt),
			(descriptor.count(game::Properties::SFX_FAILURE) > 0) ? (std::optional<std::string>(descriptor[game::Properties::SFX_FAILURE])) : (std::nullopt),
			(descriptor.count(game::Properties::INITIAL_INVENTORY) > 0) ? (std::optional<size_t>((size_t)descriptor[game::Properties::INITIAL_INVENTORY])) : (std::nullopt),
			(descriptor.count(game::Properties::COMBAT) > 0) ? (std::optional<Usage>((Usage)(int)descriptor[game::Properties::COMBAT])) : (std::nullopt),
			(descriptor.count(game::Properties::NUMBER_APPEARING) > 0) ? ((size_t)descriptor[game::Properties::NUMBER_APPEARING]) : (0),
			(descriptor.count(game::Properties::COMBAT_TEXT) > 0) ? (std::optional<std::string>(descriptor[game::Properties::COMBAT_TEXT])) : (std::nullopt)
		};
	}

}