#pragma once
#include "Game.Avatar.Statistic.h"
namespace game::avatar::Statistics
{
	int Read(const ::game::avatar::Statistic&);
	void Write(const ::game::avatar::Statistic&, int);
	int Maximum(const ::game::avatar::Statistic&);
	int Maximum(const ::game::avatar::Statistic&);
	void Reset();
	bool IsStarving();
	bool IsDead();
	void Decrease(const ::game::avatar::Statistic&, int);
	void Increase(const ::game::avatar::Statistic&, int);
}
