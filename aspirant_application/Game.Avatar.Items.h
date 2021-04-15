#pragma once
#include "Game.Item.h"
#include <map>
#include <optional>
#include <string>
#include <tuple>
namespace game::avatar::Items
{
	size_t Read(game::Item);
	void Reset();
	void Add(game::Item, size_t);
	std::map<game::Item, size_t> All();
	void Start(); 
	void Drop(std::optional<game::Item> item);
	std::optional<std::string> Use(std::optional<game::Item>);
	std::optional<std::tuple<std::string, bool>> CombatUse(std::optional<game::Item>);
}
