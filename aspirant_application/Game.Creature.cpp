#include "Game.Creature.h"
#include "Game.Data.Properties.h"
#include "Data.JSON.h"
#include "Common.Utility.h"
#include "Data.Stores.h"
#include "Common.Data.Properties.h"
namespace game::creature
{
	static std::vector<Descriptor> descriptors;

	static void Add(nlohmann::json& creatureDescriptor)
	{
		std::map<int, size_t> bribes;
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
		std::map<int, AttitudeDescriptor> attitudes;
		if (creatureDescriptor.count(game::data::Properties::ATTITUDES) > 0)
		{
			auto& amounts = creatureDescriptor[game::data::Properties::ATTITUDES];
			for (auto& amount : amounts.items())
			{
				auto& key = amount.key();
				auto& item = amount.value();
				attitudes[common::Utility::StringToInt(key)] = 
				{ 
					(Attitude)(int)item[game::data::Properties::ATTITUDE], 
					(std::string)item[game::data::Properties::SFX] 
				};
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
		std::vector<size_t> numberAppearing;
		if (creatureDescriptor.count(game::data::Properties::NUMBER_APPEARING) > 0)
		{
			auto& amounts = creatureDescriptor[game::data::Properties::NUMBER_APPEARING];
			for (auto& amount : amounts)
			{
				numberAppearing.push_back(amount);
			}
		}
		descriptors.push_back(
		{
			creatureDescriptor[game::data::Properties::IMAGE_ID],
			creatureDescriptor[game::data::Properties::HEALTH],
			creatureDescriptor[game::data::Properties::ATTACK],
			creatureDescriptor[game::data::Properties::DEFEND],
			numberAppearing,
			bribes,
			sfx,
			creatureDescriptor[game::data::Properties::ATTITUDE],
			attitudes,
			(creatureDescriptor.count(game::data::Properties::PREFER_DEAD_ENDS) > 0) ? ((bool)creatureDescriptor[game::data::Properties::PREFER_DEAD_ENDS]) : (false),
			dropWeights,
			dropCounts,
			creatureDescriptor[common::data::Properties::NAME]
		});
	}

	static bool initialized = false;

	static void Initialize()
	{
		if (!initialized)
		{
			for (auto& descriptor : ::data::Stores::GetStore(::data::Store::CREATURE_DESCRIPTORS))
			{
				Add(descriptor);
			}
			initialized = true;
		}
	}

	const Descriptor& GetDescriptor(int creature)
	{
		Initialize();
		return descriptors[creature];
	}

	const std::vector<Descriptor>& All()
	{
		Initialize();
		return descriptors;
	}
}