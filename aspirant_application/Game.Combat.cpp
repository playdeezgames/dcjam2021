#include "Application.Sounds.h"
#include "Common.Audio.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Combat.h"
#include "Game.CombatDeck.h"
#include "Game.Creatures.h"
#include "Visuals.Texts.h"
namespace game::Combat
{
	const int HOSTILE_MULTIPLIER = 2;

	void Advance()
	{
		game::Creatures::Advance(game::Avatar::GetPosition());
		CombatDeck::Deal();
	}

	static CombatResult ResolveHit()
	{
		auto damage =
			game::Creatures::DoDamage(
				game::Avatar::GetPosition(),
				game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK) +
				game::avatar::Statistics::Read(game::avatar::Statistic::STREAK));
		game::avatar::Statistics::DoAttackTimer();
		game::avatar::Statistics::Increase(game::avatar::Statistic::STREAK, 1);

		if (damage > 0)
		{
			if (game::Creatures::GetInstance(game::Avatar::GetPosition()).value().health <= 0)
			{
				return CombatResult::MONSTER_KILLED;
			}

			return CombatResult::MONSTER_HIT;
		}

		return CombatResult::MONSTER_BLOCKED;
	}

	static int AdjustAttackForAttitude(int attack, game::creature::Attitude attitude)
	{
		if (attitude == game::creature::Attitude::HOSTILE)
		{
			return attack * HOSTILE_MULTIPLIER;
		}
		return attack;
	}

	static CombatResult ResolveMiss()
	{
		game::avatar::Statistics::Write(game::avatar::Statistic::STREAK, 0);
		auto instance = game::Creatures::GetInstance(game::Avatar::GetPosition()).value();

		if (instance.attitude == game::creature::Attitude::DISTRACTED)
		{
			game::Creatures::SetAttitude(game::Avatar::GetPosition(), game::creature::Attitude::NEUTRAL);
			return CombatResult::MONSTER_DISTRACTED;
		}

		auto attack = AdjustAttackForAttitude(instance.descriptor.attack, instance.descriptor.attitude);
		auto defend = game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND);
		auto damage = attack - defend;
		game::avatar::Statistics::DoDefendTimer();

		if (damage > 0)
		{
			game::avatar::Statistics::Decrease(game::avatar::Statistic::HEALTH, damage);
			return CombatResult::HUNTER_HIT;
		}

		return CombatResult::HUNTER_BLOCKED;
	}

	static CombatResult ResolveGuess(const game::CombatDeck::Guess& guess)
	{
		if (game::CombatDeck::IsGuessCorrect(guess))
		{
			return ResolveHit();
		}
		return ResolveMiss();
	}

	static CombatResult ResolveNoGuess()
	{
		game::avatar::Statistics::Decrease(game::avatar::Statistic::HEALTH, game::Creatures::GetInstance(game::Avatar::GetPosition()).value().descriptor.attack);
		return CombatResult::HUNTER_RAN;
	}

	CombatResult Resolve(std::optional<game::CombatDeck::Guess> guess)
	{
		if (guess)
		{
			return ResolveGuess(*guess);
		}

		return ResolveNoGuess();
	}

}