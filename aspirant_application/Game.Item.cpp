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
	std::optional<std::string> Descriptor::GetSfx(const ItemSfx& itemSfx) const
	{
		auto iter = sfx.find(itemSfx);
		if (iter != sfx.end())
		{
			return iter->second;
		}
		return std::nullopt;
	}

	static std::string ItemToItemKey(const int& item)
	{
		std::stringstream ss;
		ss << (int)item;
		return ss.str();
	}

	static std::vector<int> itemIds;
	static std::vector<Descriptor> descriptors;
	static std::map<int, size_t> descriptorIndices;

	static void Initialize()
	{
		static bool initialized = false;
		if (!initialized)
		{
			for (auto& item : ::data::Stores::GetStore(::data::Store::ITEM_DESCRIPTORS).items())
			{
				int itemId = common::Utility::StringToInt(item.key());
				itemIds.push_back(itemId);
				auto& descriptor = ::data::Stores::GetStore(::data::Store::ITEM_DESCRIPTORS)[item.key()];
				std::vector<size_t> numberAppearing;
				if (descriptor.count(game::data::Properties::NUMBER_APPEARING) > 0)
				{
					for (auto& value : descriptor[game::data::Properties::NUMBER_APPEARING])
					{
						numberAppearing.push_back(value);
					}
				}
				std::optional<std::vector<size_t>> initialInventory;
				if (descriptor.count(game::data::Properties::INITIAL_INVENTORY) > 0)
				{
					std::vector<size_t> counts;
					for (auto& value : descriptor[game::data::Properties::INITIAL_INVENTORY])
					{
						counts.push_back(value);
					}
					initialInventory = counts;
				}
				std::map<ItemSfx, std::string> sfx;
				if (descriptor.count(game::data::Properties::SFX) > 0)
				{
					for (auto& item : descriptor[game::data::Properties::SFX].items())
					{
						sfx[(ItemSfx)common::Utility::StringToInt(item.key())] = item.value();
					}
				}
				descriptorIndices[itemId] = descriptors.size();
				descriptors.push_back(
				{
					descriptor[common::data::Properties::NAME],
					descriptor[game::data::Properties::IMAGE_ID],
					descriptor[game::data::Properties::TAKE_IMAGE_ID],
					descriptor[game::data::Properties::TAKE_AREA_ID],
					(descriptor.count(game::data::Properties::NON_COMBAT) > 0) ? (std::optional<Usage>((Usage)(int)descriptor[game::data::Properties::NON_COMBAT])) : (std::nullopt),
					(descriptor.count(game::data::Properties::AMOUNT) > 0) ? (std::optional<int>((int)descriptor[game::data::Properties::AMOUNT])) : (std::nullopt),
					(descriptor.count(game::data::Properties::DURATION) > 0) ? (std::optional<int>((int)descriptor[game::data::Properties::DURATION])) : (std::nullopt),
					initialInventory,
					(descriptor.count(game::data::Properties::COMBAT) > 0) ? (std::optional<Usage>((Usage)(int)descriptor[game::data::Properties::COMBAT])) : (std::nullopt),
					numberAppearing,
					(descriptor.count(game::data::Properties::COMBAT_TEXT) > 0) ? (std::optional<std::string>(descriptor[game::data::Properties::COMBAT_TEXT])) : (std::nullopt),
					(descriptor.count(game::data::Properties::LOSE_ON_TELEPORT) > 0) ? ((bool)descriptor[game::data::Properties::LOSE_ON_TELEPORT]) : (false),
					(descriptor.count(game::data::Properties::DROP_ON_USE) > 0) ? ((bool)descriptor[game::data::Properties::DROP_ON_USE]) : (false),
					(descriptor.count(game::data::Properties::BOWEL) > 0) ? (std::optional<int>((int)descriptor[game::data::Properties::BOWEL])) : (std::nullopt),
					(descriptor.count(game::data::Properties::DRUNKENNESS) > 0) ? (std::optional<int>((int)descriptor[game::data::Properties::DRUNKENNESS])) : (std::nullopt),
					sfx
				});
			}
			initialized = true;
		}
	}

	const std::vector<int>& All()
	{
		Initialize();
		return itemIds;
	}

	const Descriptor& GetDescriptor(int item)
	{
		return descriptors[descriptorIndices.find(item)->second];
	}
}