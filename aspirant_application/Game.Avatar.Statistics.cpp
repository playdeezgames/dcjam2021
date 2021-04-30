#include "Game.Avatar.Statistics.h"
#include <map>
#include "json.hpp"
#include "Game.Data.Properties.h"
#include "Game.Avatar.h"
#include "Data.JSON.h"
#include "Common.Data.Properties.h"
#include <sstream>
#include "Common.Utility.h"
#include "Data.Stores.h"
namespace game::avatar::Statistics
{
	static const nlohmann::json& GetStatistic(game::avatar::Statistic statistic)
	{
		std::stringstream ss;
		ss << (int)statistic;
		return ::data::Stores::GetStore(::data::Store::STATISTICS)[ss.str()];
	}

	nlohmann::json& GetAvatarStatistics()
	{
		auto& avatar = game::Avatar::GetAvatar();
		if (avatar.count(game::data::Properties::STATISTICS) == 0)
		{
			avatar[game::data::Properties::STATISTICS] = nlohmann::json();
		}
		return avatar[game::data::Properties::STATISTICS];
	}

	int Read(const ::game::avatar::Statistic& statistic)
	{
		return GetAvatarStatistics()[(int)statistic];
	}

	int Minimum(const ::game::avatar::Statistic& statistic)
	{
		return (int)GetStatistic(statistic)[common::data::Properties::MINIMUM];
	}

	int Maximum(const ::game::avatar::Statistic& statistic)
	{
		return (int)GetStatistic(statistic)[common::data::Properties::MAXIMUM];
	}

	int Default(const ::game::avatar::Statistic& statistic)
	{
		return (int)GetStatistic(statistic)[common::data::Properties::DEFAULT];
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

	bool IsMaximum(const ::game::avatar::Statistic& statistic)
	{
		return Read(statistic) == Maximum(statistic);
	}

	void Reset()
	{
		for (auto& item : ::data::Stores::GetStore(::data::Store::STATISTICS).items())
		{
			game::avatar::Statistic statistic = (game::avatar::Statistic)common::Utility::StringToInt(item.key());
			Write(statistic, Default(statistic));
		}
	}

	void Start()
	{
		Reset();
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