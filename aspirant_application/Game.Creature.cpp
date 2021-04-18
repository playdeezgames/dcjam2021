#include "Game.Creature.h"
#include "Game.Data.Properties.h"
#include "Data.JSON.h"
#include "Common.Utility.h"
namespace game::creature
{
	nlohmann::json descriptors;

	Descriptor GetDescriptor(int creature)
	{
		std::map<int, size_t> bribes;
		auto& creatureDescriptor = descriptors[(int)creature];
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

	void InitializeFromFile(const std::string& filename)
	{
		descriptors = ::data::JSON::Load(filename);
	}

	std::vector<int> All()
	{
		std::vector<int> result;
		for (auto& descriptor : descriptors)
		{
			result.push_back((int)descriptor[game::data::Properties::INDEX]);
		}
		return result;
	}
}
