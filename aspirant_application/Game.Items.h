#pragma once
#include "Game.Item.h"
#include "Common.XY.h"
namespace game::Items
{
	bool IsPresent(const game::Item&, const common::XY<size_t>&);
}
