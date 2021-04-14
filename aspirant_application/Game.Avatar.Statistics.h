#pragma once
#include "Game.Avatar.Statistic.h"
namespace game::avatar::Statistics
{
	int Read(const ::game::avatar::Statistic&);
	void Write(const ::game::avatar::Statistic&, int);
	int Maximum(const ::game::avatar::Statistic&);
	int Default(const ::game::avatar::Statistic&);
	bool IsMinimum(const ::game::avatar::Statistic&);
	void Reset();
	bool IsMinimum(const ::game::avatar::Statistic&);
	void Decrease(const ::game::avatar::Statistic&, int);
	void Increase(const ::game::avatar::Statistic&, int);
	void DoAttackTimer();
	void DoDefendTimer();
}
