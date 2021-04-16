#include "Game.Creature.h"
#include "Game.Properties.h"
#include "Data.JSON.h"
namespace game::creature
{
	nlohmann::json descriptors;

	Descriptor GetDescriptor(int creature)
	{
		return
		{
			descriptors[(int)creature][game::Properties::IMAGE_ID],
			descriptors[(int)creature][game::Properties::HEALTH],
			descriptors[(int)creature][game::Properties::ATTACK],
			descriptors[(int)creature][game::Properties::DEFEND],
			descriptors[(int)creature][game::Properties::FOOD_BRIBE],
			descriptors[(int)creature][game::Properties::MONEY_BRIBE],
			descriptors[(int)creature][game::Properties::NUMBER_APPEARING]
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