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
		return
		{
			creatureDescriptor[game::data::Properties::IMAGE_ID],
			creatureDescriptor[game::data::Properties::HEALTH],
			creatureDescriptor[game::data::Properties::ATTACK],
			creatureDescriptor[game::data::Properties::DEFEND],
			creatureDescriptor[game::data::Properties::NUMBER_APPEARING],
			bribes
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
