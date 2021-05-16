#pragma once
#include "Game.Item.h"
#include <map>
#include <optional>
#include <string>
#include <tuple>
#include "Game.h"
namespace game::avatar::Items
{
	size_t Read(int);
	void Reset(const game::Difficulty&);
	void Add(int, size_t);
	size_t Remove(int, size_t);
	std::map<int, size_t> All();
	void Start(); 
	void Drop(std::optional<int> item);
	std::optional<std::string> Use(std::optional<int>);
	enum class CombatUseResult
	{
		REFRESH,
		SKIP
	};
	std::optional<std::tuple<std::string, CombatUseResult>> CombatUse(std::optional<int>);
	bool HasItems(const std::map<int, size_t>&);
	void RemoveItems(const std::map<int, size_t>&);
	void ReceiveItems(const std::map<int, size_t>&);
}
