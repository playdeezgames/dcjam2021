#pragma once
#include "Common.XY.h"
#include <optional>
#include <string>
#include <vector>
namespace game
{
	enum class Creature
	{
		IMP,
		TROGLODYTE,
		BRAIN_TENTACLE,
		BULLY_MINION,
		EVIL_BAT,
		EVIL_MINION,
		WEREWOLF,
		WYVERN
	};

}
namespace game::creature
{
	struct Descriptor
	{
		std::string imageId;
		int maximumHealth;
		int attack;
		int defend;
		int foodBribe;
		int moneyBribe;
		size_t numberAppearing;
	};

	Descriptor GetDescriptor(game::Creature);
	void InitializeFromFile(const std::string&);
	std::vector<game::Creature> All();
}
