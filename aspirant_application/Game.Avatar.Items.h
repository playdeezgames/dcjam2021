#pragma once
#include "Game.Item.h"
namespace game::avatar::Items
{
	size_t Read(game::Item);
	void Reset();
	void Add(game::Item, size_t);
}
