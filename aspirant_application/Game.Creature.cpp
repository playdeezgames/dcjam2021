#include "Game.Creature.h"
#include "Game.Data.Properties.h"
#include "Data.JSON.h"
#include "Common.Utility.h"
#include "Data.Stores.h"
namespace game::creature
{
	Descriptor GetDescriptor(int creature)
	{
		std::map<int, size_t> bribes;
		auto& creatureDescriptor = ::data::Stores::GetStore(::data::Store::CREATURE_DESCRIPTORS)[(int)creature];
		if (creatureDescriptor.count(game::data::Properties::BRIBES) > 0)
		{
			auto& amounts = creatureDescriptor[game::data::Properties::BRIBES];
			for (auto& amount : amounts.items())
			{
				bribes[common::Utility::StringToInt(amount.key())] = (size_t)amount.value();
			}
		}
		std::map<Sfx, std::string> sfx;
		if (creatureDescriptor.count(game::data::Properties::SFX) > 0)
		{
			auto& amounts = creatureDescriptor[game::data::Properties::SFX];
			for (auto& amount : amounts.items())
			{
				sfx[(Sfx)common::Utility::StringToInt(amount.key())] = amount.value();
			}
		}
		std::map<int, Attitude> attitudes;
		if (creatureDescriptor.count(game::data::Properties::ATTITUDES) > 0)
		{
			auto& amounts = creatureDescriptor[game::data::Properties::ATTITUDES];
			for (auto& amount : amounts.items())
			{
				attitudes[common::Utility::StringToInt(amount.key())] = (Attitude)(int)amount.value();
			}
		}
		std::map<std::optional<int>, size_t> dropWeights;
		std::map<int, size_t> dropCounts;
		if (creatureDescriptor.count(game::data::Properties::DROPS) > 0)
		{
			auto& drops = creatureDescriptor[game::data::Properties::DROPS];
			for (auto& drop : drops.items())
			{
				if (drop.key() == game::data::Properties::NONE)
				{
					dropWeights[std::nullopt] = (size_t)drop.value();
				}
				else 
				{
					int itemId = common::Utility::StringToInt(drop.key());
					auto& entry = drop.value();
					size_t weight = entry[game::data::Properties::WEIGHT];
					size_t quantity = entry[game::data::Properties::QUANTITY];
					dropWeights[itemId] = weight;
					dropCounts[itemId] = quantity;
				}
			}
		}
		return
		{
			creatureDescriptor[game::data::Properties::IMAGE_ID],
			creatureDescriptor[game::data::Properties::HEALTH],
			creatureDescriptor[game::data::Properties::ATTACK],
			creatureDescriptor[game::data::Properties::DEFEND],
			creatureDescriptor[game::data::Properties::NUMBER_APPEARING],
			bribes,
			sfx,
			creatureDescriptor[game::data::Properties::ATTITUDE],
			attitudes,
			(creatureDescriptor.count(game::data::Properties::PREFER_DEAD_ENDS)>0) ? ((bool)creatureDescriptor[game::data::Properties::PREFER_DEAD_ENDS]) : (false),
			dropWeights,
			dropCounts
		};
	}

	std::vector<int> All()
	{
		std::vector<int> result;
		for (auto& descriptor : ::data::Stores::GetStore(::data::Store::CREATURE_DESCRIPTORS))
		{
			result.push_back((int)descriptor[game::data::Properties::INDEX]);
		}
		return result;
	}
}
