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

	void Start(const std::string&);
}
