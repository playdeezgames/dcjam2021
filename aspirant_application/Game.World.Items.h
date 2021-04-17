#pragma once
#include "Game.Item.h"
#include "Common.XY.h"
#include <map>
namespace game::world::Items
{
	bool IsPresent(const int&, const common::XY<size_t>&);//TODO: should xy be first?
	const std::map<int, size_t>& FloorInventory(const common::XY<size_t>&);
	size_t Remove(const int&, size_t, const common::XY<size_t>&);//TODO: should xy be first?
	void Add(const int&, size_t, const common::XY<size_t>&);//TODO: should xy be first?
}
