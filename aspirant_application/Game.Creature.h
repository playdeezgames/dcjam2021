#pragma once
#include "Common.XY.h"
#include <optional>
#include <string>
#include <vector>
#include <map>
#include "Game.Item.h"
namespace game::creature
{
	enum class Sfx
	{
		HIT,
		DEATH,
		BLOCK,
		BRIBE,
		NO_BRIBE
	};
	struct Descriptor
	{
		std::string imageId;
		int maximumHealth;
		int attack;
		int defend;
		size_t numberAppearing;
		std::map<int, size_t> bribes;
		std::map<Sfx, std::string> sfx;
	};

	Descriptor GetDescriptor(int);
	std::vector<int> All();
}
