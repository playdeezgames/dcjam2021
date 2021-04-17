#pragma once
#include "Game.Item.h"
#include "Common.XY.h"
#include <map>
namespace game::world::Items
{
	bool IsPresent(const common::XY<size_t>&,const int&);
	const std::map<int, size_t>& FloorInventory(const common::XY<size_t>&);
	size_t Remove(const common::XY<size_t>&,const int&, size_t);
	void Add(const common::XY<size_t>&,const int&, size_t);
}
