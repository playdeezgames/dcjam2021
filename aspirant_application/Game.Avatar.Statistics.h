#pragma once
#include "Game.Avatar.Statistic.h"
#include <string>
namespace game::avatar::Statistics
{
	int Read(const ::game::avatar::Statistic&);
	void Write(const ::game::avatar::Statistic&, int);
	bool IsMinimum(const ::game::avatar::Statistic&);
	void Decrease(const ::game::avatar::Statistic&, int);
	void Increase(const ::game::avatar::Statistic&, int);
	void Reset();
	void DoAttackTimer();
	void DoDefendTimer();
	int Maximum(const ::game::avatar::Statistic&);
	int Default(const ::game::avatar::Statistic&);
	void InitializeFromFile(const std::string&);
}
