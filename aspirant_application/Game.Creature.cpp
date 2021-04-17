#include "Game.Creature.h"
#include "Game.Properties.h"
#include "Data.JSON.h"
#include "Common.Utility.h"
namespace game::creature
{
	nlohmann::json descriptors;

	Descriptor GetDescriptor(int creature)
	{
		std::map<game::Item, size_t> bribes;
		auto& creatureDescriptor = descriptors[(int)creature];
		if (creatureDescriptor.count(game::Properties::BRIBES) > 0)
		{
			auto& amounts = creatureDescriptor[game::Properties::BRIBES];
			for (auto& amount : amounts.items())
			{
				bribes[(game::Item)common::Utility::StringToInt(amount.key())] = (size_t)amount.value();
			}
		}
		return
		{
			creatureDescriptor[game::Properties::IMAGE_ID],
			creatureDescriptor[game::Properties::HEALTH],
			creatureDescriptor[game::Properties::ATTACK],
			creatureDescriptor[game::Properties::DEFEND],
			creatureDescriptor[game::Properties::FOOD_BRIBE],
			creatureDescriptor[game::Properties::MONEY_BRIBE],
			creatureDescriptor[game::Properties::NUMBER_APPEARING],
			bribes
		};
	}

	void InitializeFromFile(const std::string& filename)
	{
		descriptors = data::JSON::Load(filename);
	}

	std::vector<int> All()
	{
		std::vector<int> result;
		for (auto& descriptor : descriptors)
		{
			result.push_back((int)descriptor[game::Properties::INDEX]);
		}
		return result;
	}
}
