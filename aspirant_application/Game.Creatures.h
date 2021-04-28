#pragma once
#include "Game.Creature.h"
#include "Common.XY.h"
#include <optional>
#include <string>
namespace game::Creatures
{
	struct Instance
	{
		int creature;
		int health;
		game::creature::Descriptor descriptor;
		game::creature::Attitude attitude;
	};
	std::optional<Instance> GetInstance(const common::XY<size_t>&);
	bool AnyLeft();
	std::optional<int> DoDamage(const common::XY<size_t>&, int);
	void SetAttitude(const common::XY<size_t>&, game::creature::Attitude);
	std::optional<std::string> ChangeAttitude(const common::XY<size_t>&, int);
	void Remove(const common::XY<size_t>&);
	void Advance(const common::XY<size_t>&);
}
