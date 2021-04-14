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

	static bool IsGuessCorrect(Guess guess)//TODO: move this to CombatDeck
	{
		switch (guess)
		{
		case Guess::HIGHER:
			return std::get<0>(CombatDeck::GetNextCard()) > std::get<0>(CombatDeck::GetCurrentCard());
		case Guess::LOWER:
			return std::get<0>(CombatDeck::GetNextCard()) < std::get<0>(CombatDeck::GetCurrentCard());
		}
		return false;
	}

	static void DoAttackTimer()//TODO: move this to avatar statistics
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

	static void DoDefendTimer()//TODO: move this to avatar statistics
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

	CombatResult Resolve(std::optional<Guess> guess)
	{
		if (guess)
		{
			if (IsGuessCorrect(guess.value()))
			{
				auto damage =
					game::Creatures::DoDamage(
						game::Avatar::GetPosition(),
						game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK));
				if (damage > 0)
				{
					if (game::Creatures::IsDead(game::Avatar::GetPosition()).value())
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
				DoAttackTimer();
			}
			else
			{
				auto attack = game::Creatures::GetAttack(game::Avatar::GetPosition()).value();
				auto defend = game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND);
				auto damage = attack - defend;
				if (damage > 0)
				{
					game::avatar::Statistics::Decrease(game::avatar::Statistic::HEALTH, damage);
					return CombatResult::HUNTER_HIT;
				}
				else
				{
					return CombatResult::HUNTER_BLOCKED;
				}
				DoDefendTimer();
			}
		}
		else
		{
			game::avatar::Statistics::Decrease(game::avatar::Statistic::HEALTH, game::Creatures::GetAttack(game::Avatar::GetPosition()).value());
			return CombatResult::HUNTER_RAN;
		}
	}

}