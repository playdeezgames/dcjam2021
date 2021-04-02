#include "Game.Avatar.Statistics.h"
#include <map>
namespace game::avatar::Statistics
{
	const std::map<::game::avatar::Statistic, int> maximums =
	{
		{::game::avatar::Statistic::HEALTH, 100},
		{::game::avatar::Statistic::HUNGER, 100}
	};

	const std::map<::game::avatar::Statistic, int> minimums =
	{
		{::game::avatar::Statistic::HEALTH, 0},
		{::game::avatar::Statistic::HUNGER, 0}
	};

	static std::map<::game::avatar::Statistic, int> current;

	int Read(const ::game::avatar::Statistic& statistic)
	{
		return current[statistic];
	}

	int Minimum(const ::game::avatar::Statistic& statistic)
	{
		return minimums.find(statistic)->second;
	}

	int Maximum(const ::game::avatar::Statistic& statistic)
	{
		return maximums.find(statistic)->second;
	}

	void Write(const ::game::avatar::Statistic& statistic, int value)
	{
		const auto minimum = Minimum(statistic);
		const auto maximum = Maximum(statistic);
		current[statistic] =
			(value < minimum) ? (minimum) :
			(value > maximum) ? (maximum) :
			(value);
	}

	void Reset()
	{
		current = maximums;
	}

	bool IsStarving()
	{
		return Read(Statistic::HUNGER) == Minimum(Statistic::HUNGER);
	}

	bool IsDead()
	{
		return Read(Statistic::HEALTH) == Minimum(Statistic::HEALTH);
	}

	void Decrease(const ::game::avatar::Statistic& statistic, int delta)
	{
		Write(statistic, Read(statistic) - delta);
	}

}