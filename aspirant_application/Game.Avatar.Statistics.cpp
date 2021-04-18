#include "Game.Avatar.Statistics.h"
#include <map>
#include "json.hpp"
#include "Game.Properties.h"
#include "Game.Avatar.h"
#include "Data.JSON.h"
#include "Common.Properties.h"
#include <sstream>
#include "Common.Utility.h"
namespace game::avatar::Statistics
{
	nlohmann::json statistics;

	void InitializeFromFile(const std::string& fileName)
	{
		statistics = data::JSON::Load(fileName);
	}

	static const nlohmann::json& GetStatistic(game::avatar::Statistic statistic)
	{
		std::stringstream ss;
		ss << (int)statistic;
		return statistics[ss.str()];
	}

	nlohmann::json& GetAvatarStatistics()
	{
		auto& avatar = game::Avatar::GetAvatar();
		if (avatar.count(game::Properties::STATISTICS) == 0)
		{
			avatar[game::Properties::STATISTICS] = nlohmann::json();
		}
		return avatar[game::Properties::STATISTICS];
	}

	int Read(const ::game::avatar::Statistic& statistic)
	{
		return GetAvatarStatistics()[(int)statistic];
	}

	int Minimum(const ::game::avatar::Statistic& statistic)
	{
		return (int)GetStatistic(statistic)[common::Properties::MINIMUM];
	}

	int Maximum(const ::game::avatar::Statistic& statistic)
	{
		return (int)GetStatistic(statistic)[common::Properties::MAXIMUM];
	}

	int Default(const ::game::avatar::Statistic& statistic)
	{
		return (int)GetStatistic(statistic)[common::Properties::DEFAULT];
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
		for (auto& item : statistics.items())
		{
			game::avatar::Statistic statistic = (game::avatar::Statistic)common::Utility::StringToInt(item.key());
			Write(statistic, Default(statistic));
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

	void DoAttackTimer()
	{
		auto timer = game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK_TIMER);
		if (timer > 0)
		{
			timer--;
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK_TIMER, timer);
			if (timer == 0)
			{
				game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK, game::avatar::Statistics::Default(game::avatar::Statistic::ATTACK));
			}
		}
	}

	void DoDefendTimer()
	{
		auto timer = game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND_TIMER);
		if (timer > 0)
		{
			timer--;
			game::avatar::Statistics::Write(game::avatar::Statistic::DEFEND_TIMER, timer);
			if (timer == 0)
			{
				game::avatar::Statistics::Write(game::avatar::Statistic::DEFEND, game::avatar::Statistics::Default(game::avatar::Statistic::DEFEND));
			}
		}
	}
}