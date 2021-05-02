#include "Game.Combat.h"
#include "Game.CombatDeck.h"
#include "Game.Avatar.h"
#include "Game.Creatures.h"
#include "Game.Avatar.Statistics.h"
#include "Application.Sounds.h"
#include "Common.Audio.h"
#include "Visuals.Texts.h"
namespace game::Combat
{
	void Advance()
	{
		game::Creatures::Advance(game::Avatar::GetPosition());
		CombatDeck::Deal();
	}

	CombatResult Resolve(std::optional<game::CombatDeck::Guess> guess)
	{
		if (guess)
		{
			if (game::CombatDeck::IsGuessCorrect(guess.value()))
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
					if (game::Creatures::GetInstance(game::Avatar::GetPosition()).value().health<=0)
					{
						return CombatResult::MONSTER_KILLED;
					}
					else
					{
						return CombatResult::MONSTER_HIT;
					}
				}
				else
				{
					return CombatResult::MONSTER_BLOCKED;
				}
			}
			else
			{
				game::avatar::Statistics::Write(game::avatar::Statistic::STREAK, 0);
				auto instance = game::Creatures::GetInstance(game::Avatar::GetPosition()).value();
				if (instance.attitude == game::creature::Attitude::DISTRACTED)
				{
					game::Creatures::SetAttitude(game::Avatar::GetPosition(), game::creature::Attitude::NEUTRAL);
					return CombatResult::MONSTER_DISTRACTED;
				}
				else
				{
					auto attack = instance.descriptor.attack;
					auto defend = game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND);
					auto damage = attack - defend;
					game::avatar::Statistics::DoDefendTimer();
					if (damage > 0)
					{
						game::avatar::Statistics::Decrease(game::avatar::Statistic::HEALTH, damage);
						return CombatResult::HUNTER_HIT;
					}
					else
					{
						return CombatResult::HUNTER_BLOCKED;
					}
				}
			}
		}
		else
		{
			game::avatar::Statistics::Decrease(game::avatar::Statistic::HEALTH, game::Creatures::GetInstance(game::Avatar::GetPosition()).value().descriptor.attack);
			return CombatResult::HUNTER_RAN;
		}
	}

}