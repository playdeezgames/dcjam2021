#pragma once
#include "Game.Item.h"
#include <map>
#include <optional>
#include <string>
namespace game::avatar::Items
{
	size_t Read(game::Item);
	void Reset();
	void Add(game::Item, size_t);
	size_t Remove(game::Item, size_t);
	std::map<game::Item, size_t> All();
	void Start(); 
	void DropItem(std::optional<game::Item> item);
	std::optional<std::string> UseItem(std::optional<game::Item> item);
}
