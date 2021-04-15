#pragma once
#include "Common.XY.h"
#include <optional>
#include <string>
#include <vector>
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

	Descriptor GetDescriptor(int);
	void InitializeFromFile(const std::string&);
	std::vector<int> All();
}
