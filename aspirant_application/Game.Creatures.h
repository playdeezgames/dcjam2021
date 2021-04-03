#pragma once
#include "Game.Creature.h"
#include "Common.XY.h"
#include <optional>
#include <string>
namespace game::Creatures
{
	std::optional<game::Creature> Read(const common::XY<size_t>&);
	void Start(const std::string&);
}
