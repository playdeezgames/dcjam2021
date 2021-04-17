#pragma once
#include "Game.Item.h"
#include <map>
#include <optional>
#include <string>
#include <tuple>
namespace game::avatar::Items
{
	size_t Read(int);
	void Reset();
	void Add(int, size_t);
	std::map<int, size_t> All();
	void Start(); 
	void Drop(std::optional<int> item);
	std::optional<std::string> Use(std::optional<int>);
	std::optional<std::tuple<std::string, bool>> CombatUse(std::optional<int>);
}
