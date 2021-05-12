#include <sstream>
#include "Application.OnEnter.h"
#include "Visuals.Texts.h"
#include "Game.Avatar.h"
#include "Game.Creatures.h"
#include "Game.Avatar.Statistics.h"
namespace sublayout::EnemyStats
{
	const std::string LAYOUT_NAME = "Sublayout.EnemyStats";
	const std::string HEALTH_TEXT_ID = "EnemyHealth";
	const std::string ATTACK_TEXT_ID = "Attack";
	const std::string STREAK_TEXT_ID = "Streak";
	const std::string DEFEND_TEXT_ID = "Defend";
	const std::string TEXT_ENEMY_ATTITUDE = "EnemyAttitude";

	const std::map<game::creature::Attitude, std::string> attitudeTexts =
	{
		{ game::creature::Attitude::NEUTRAL, "" },
		{ game::creature::Attitude::UNIMPRESSED, "Unimpressed" },
		{ game::creature::Attitude::AMOROUS, "Feeling randy!" },
		{ game::creature::Attitude::DISGUST, "Disgusted" },
		{ game::creature::Attitude::HOSTILE, "Angry" },
		{ game::creature::Attitude::EMBARRASSED, "*BLUSHING*" },
		{ game::creature::Attitude::DISTRACTED, "Distracted" }
	};

	static void UpdateAttitude()
	{
		std::stringstream ss;
		auto position = game::Avatar::GetPosition();
		auto creature = game::Creatures::GetInstance(position);
		if (creature)
		{
			ss << attitudeTexts.find(creature.value().attitude)->second;
		}
		::visuals::Texts::SetText(LAYOUT_NAME, TEXT_ENEMY_ATTITUDE, ss.str());
	}

	static void UpdateHealth()
	{
		std::stringstream ss;
		ss << "Enemy: ";
		auto position = game::Avatar::GetPosition();
		auto creature = game::Creatures::GetInstance(position);
		if (creature)
		{
			auto health = creature.value().health;
			if (health > 0)
			{
				ss << health << "/";
				ss << creature.value().descriptor.maximumHealth;
			}
			else
			{
				ss << "DEAD!";
			}
			::visuals::Texts::SetText(LAYOUT_NAME, HEALTH_TEXT_ID, ss.str());
		}
	}

	static void UpdateAttack()
	{
		auto attack = game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK) + game::avatar::Statistics::Read(game::avatar::Statistic::STREAK);
		auto timer = game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK_TIMER);
		std::stringstream ss;
		ss << "Courage: " << attack;
		if (timer > 0)
		{
			ss << "(" << timer << ")";
		}
		::visuals::Texts::SetText(LAYOUT_NAME, ATTACK_TEXT_ID, ss.str());
	}

	static void UpdateStreak()
	{
		auto streak = game::avatar::Statistics::Read(game::avatar::Statistic::STREAK);
		std::stringstream ss;
		ss << "Streak: " << streak;
		::visuals::Texts::SetText(LAYOUT_NAME, STREAK_TEXT_ID, ss.str());
	}

	static void UpdateDefend()
	{
		auto defend = game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND);
		auto timer = game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND_TIMER);
		std::stringstream ss;
		ss << "Alertness: " << defend;
		if (timer > 0)
		{
			ss << "(" << timer << ")";
		}
		::visuals::Texts::SetText(LAYOUT_NAME, DEFEND_TEXT_ID, ss.str());
	}

	const std::vector<::UIState> states =
	{
		::UIState::IN_PLAY_COMBAT,
		::UIState::IN_PLAY_COMBAT_RESULT
	};

	void Start()
	{
		for (auto state : states)
		{
			::application::OnEnter::AddHandler(state, UpdateHealth);
			::application::OnEnter::AddHandler(state, UpdateAttack);
			::application::OnEnter::AddHandler(state, UpdateDefend);
			::application::OnEnter::AddHandler(state, UpdateAttitude);
			::application::OnEnter::AddHandler(state, UpdateStreak);
		}
	}
}