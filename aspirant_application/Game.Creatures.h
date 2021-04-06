#pragma once
#include "Game.Creature.h"
#include "Common.XY.h"
#include <optional>
#include <string>
namespace game::Creatures
{
	std::optional<game::Creature> Read(const common::XY<size_t>&);
	std::optional<int> GetMaximumHealth(const common::XY<size_t>&);
	std::optional<int> GetHealth(const common::XY<size_t>&);
	std::string GetImageId(game::Creature);
	void DecreaseHealth(const common::XY<size_t>&, int);
	std::optional<bool> IsDead(const common::XY<size_t>&);
	void Advance(const common::XY<size_t>&);
	std::optional<int> GetAttack(const common::XY<size_t>&);
	std::optional<int> GetDefend(const common::XY<size_t>&);
	std::optional<int> GetFoodBribe(const common::XY<size_t>&);
	std::optional<int> GetMoneyBribe(const common::XY<size_t>&);
	void Remove(const common::XY<size_t>&);
	bool AnyLeft();

	void Start(const std::string&);
}
