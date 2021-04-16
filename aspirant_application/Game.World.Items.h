#pragma once
#include "Game.Item.h"
#include "Common.XY.h"
#include <map>
namespace game::world::Items
{
	bool IsPresent(const game::Item&, const common::XY<size_t>&);//TODO: should xy be first?
	const std::map<game::Item, size_t>& FloorInventory(const common::XY<size_t>&);
	size_t Remove(const game::Item&, size_t, const common::XY<size_t>&);//TODO: should xy be first?
	void Add(const game::Item&, size_t, const common::XY<size_t>&);//TODO: should xy be first?
}
