#include "Game.Avatar.Statistics.h"
#include <map>
#include "json.hpp"
#include "Game.Properties.h"
namespace game::Avatar
{
	nlohmann::json& GetAvatar();
}
namespace game::avatar::Statistics
{
	nlohmann::json& GetAvatarStatistics()
	{
		auto& avatar = game::Avatar::GetAvatar();
		if (avatar.count(game::Properties::STATISTICS) == 0)
		{
			avatar[game::Properties::STATISTICS] = nlohmann::json();
		}
		return avatar[game::Properties::STATISTICS];
	}

	const std::map<::game::avatar::Statistic, int> maximums =
	{
		{::game::avatar::Statistic::HEALTH, 100},
		{::game::avatar::Statistic::HUNGER, 100},
		{::game::avatar::Statistic::ATTACK, 50},
		{::game::avatar::Statistic::DEFEND, 25},
		{::game::avatar::Statistic::ATTACK_TIMER, 1000},
		{::game::avatar::Statistic::DEFEND_TIMER, 1000}
	};

	const std::map<::game::avatar::Statistic, int> minimums =
	{
		{::game::avatar::Statistic::HEALTH, 0},
		{::game::avatar::Statistic::HUNGER, 0},
		{::game::avatar::Statistic::ATTACK, 10},
		{::game::avatar::Statistic::DEFEND, 0},
		{::game::avatar::Statistic::ATTACK_TIMER, 0},
		{::game::avatar::Statistic::DEFEND_TIMER, 0}
	};

	const std::map<::game::avatar::Statistic, int> initials =
	{
		{::game::avatar::Statistic::HEALTH, 100},
		{::game::avatar::Statistic::HUNGER, 100},
		{::game::avatar::Statistic::ATTACK, 10},
		{::game::avatar::Statistic::DEFEND, 0},
		{::game::avatar::Statistic::ATTACK_TIMER, 0},
		{::game::avatar::Statistic::DEFEND_TIMER, 0}
	};

	int Read(const ::game::avatar::Statistic& statistic)
	{
		return GetAvatarStatistics()[(int)statistic];
	}

	int Minimum(const ::game::avatar::Statistic& statistic)
	{
		return minimums.find(statistic)->second;
	}

	int Maximum(const ::game::avatar::Statistic& statistic)
	{
		return maximums.find(statistic)->second;
	}

	int Default(const ::game::avatar::Statistic& statistic)
	{
		return initials.find(statistic)->second;
	}

	void Write(const ::game::avatar::Statistic& statistic, int value)
	{
		const auto minimum = Minimum(statistic);
		const auto maximum = Maximum(statistic);
		GetAvatarStatistics()[(int)statistic] =
			(value < minimum) ? (minimum) :
			(value > maximum) ? (maximum) :
			(value);
	}

	bool IsMinimum(const ::game::avatar::Statistic& statistic)
	{
		return Read(statistic) == Minimum(statistic);
	}

	void Reset()
	{
		for (auto& initial : initials)
		{
			Write(initial.first, initial.second);
		}
	}

	void Decrease(const ::game::avatar::Statistic& statistic, int delta)
	{
		Write(statistic, Read(statistic) - delta);
	}

	void Increase(const ::game::avatar::Statistic& statistic, int delta)
	{
		Decrease(statistic, -delta);
	}
}