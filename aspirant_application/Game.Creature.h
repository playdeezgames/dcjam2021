#pragma once
#include "Common.XY.h"
#include <optional>
#include <string>
#include <vector>
#include <map>
#include "Game.Item.h"
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
		std::map<game::Item, size_t> bribes;
	};

	Descriptor GetDescriptor(int);
	void InitializeFromFile(const std::string&);
	std::vector<int> All();
}
