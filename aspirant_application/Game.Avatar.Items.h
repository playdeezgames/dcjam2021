#pragma once
#include "Game.Item.h"
#include <map>
namespace game::avatar::Items
{
	size_t Read(game::Item);
	void Reset();
	void Add(game::Item, size_t);
	const std::map<game::Item, size_t>& All();
}
