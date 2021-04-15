#pragma once
#include "Common.XY.h"
#include <optional>
#include <string>
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
	};

	Descriptor GetDescriptor(game::Creature);
	void InitializeFromFile(const std::string&);
}
