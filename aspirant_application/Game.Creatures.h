#pragma once
#include "Game.Creature.h"
#include "Common.XY.h"
#include <optional>
#include <string>
namespace game::Creatures
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
	void InitializeFromFile(const std::string&);//descriptor

	std::optional<int> GetHealth(const common::XY<size_t>&);//instance+descriptor
	std::optional<bool> IsDead(const common::XY<size_t>&);//instance+descriptor
	
	bool AnyLeft();//instances
	std::optional<game::Creature> Read(const common::XY<size_t>&);//instance
	std::optional<int> DoDamage(const common::XY<size_t>&, int);//instance
	void Remove(const common::XY<size_t>&);//instance
	void Advance(const common::XY<size_t>&);//instance
}
