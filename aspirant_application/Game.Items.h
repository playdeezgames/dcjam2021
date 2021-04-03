#pragma once
#include "Game.Item.h"
#include "Common.XY.h"
#include <map>
#include <string>
namespace game::Items
{
	bool IsPresent(const game::Item&, const common::XY<size_t>&);
	const std::map<game::Item, size_t>& FloorInventory(const common::XY<size_t>&);
	size_t Remove(const game::Item&, size_t, const common::XY<size_t>&);
	const std::string& GetName(const game::Item&);
}
