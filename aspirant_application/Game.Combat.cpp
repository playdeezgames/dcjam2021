#include "Game.Combat.h"
#include "Game.CombatDeck.h"
#include "Game.Avatar.h"
#include "Game.Creatures.h"
#include "Game.Avatar.Statistics.h"
#include "Application.Sounds.h"
#include "Common.Audio.h"
#include "Graphics.Texts.h"
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
						game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK));
				game::avatar::Statistics::DoAttackTimer();
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
				auto attack = game::creature::GetDescriptor(game::Avatar::GetPosition()).value().attack;
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
		else
		{
			game::avatar::Statistics::Decrease(game::avatar::Statistic::HEALTH, game::creature::GetDescriptor(game::Avatar::GetPosition()).value().attack);
			return CombatResult::HUNTER_RAN;
		}
	}

}