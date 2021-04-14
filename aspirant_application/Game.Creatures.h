#pragma once
#include "Game.Creature.h"
#include "Common.XY.h"
#include <optional>
#include <string>
namespace game::creature
{
	struct Descriptor
	{
		std::string imageId;
		int maximumHealth;
		int attack;
		int defend;
		int foodBribe;
		int moneyBribe;
	};

	Descriptor GetDescriptor(game::Creature);
	std::optional<Descriptor> GetDescriptor(const common::XY<size_t>&);
	void InitializeFromFile(const std::string&);
}
namespace game::Creatures
{
	struct Instance
	{
		game::Creature creature;
		int health;
		game::creature::Descriptor descriptor;
	};
	std::optional<Instance> GetInstance(const common::XY<size_t>&);
	bool AnyLeft();//instances
	std::optional<int> DoDamage(const common::XY<size_t>&, int);//instance
	void Remove(const common::XY<size_t>&);//instance
	void Advance(const common::XY<size_t>&);//instance
}
