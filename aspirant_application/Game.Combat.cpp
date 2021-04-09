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
	const std::string LAYOUT_NAME = "State.InPlay.CombatResult";
	const std::string RESULT_TEXT_ID = "Result";
	const std::string HIT_MONSTER = "You hit it!";
	const std::string KILL_MONSTER = "You killed it!";
	const std::string GOT_HIT = "It hit you!";
	const std::string BLOCKED_HIT = "You block!";
	const std::string MISSED_MONSTER = "It blocked!";


	void SetCombatResultText(const std::string& text)
	{
		graphics::Texts::SetText(LAYOUT_NAME, RESULT_TEXT_ID, text);
	}

	void Advance()
	{
		game::Creatures::Advance(game::Avatar::GetPosition());
		CombatDeck::Deal();
	}

	static bool IsGuessCorrect(Guess guess)
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

	static void DoAttackTimer()
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

	static void DoDefendTimer()
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

	void Resolve(std::optional<Guess> guess)
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
						SetCombatResultText(KILL_MONSTER);
						common::audio::PlaySound(application::Sounds::DEAD_MONSTER);
					}
					else
					{
						SetCombatResultText(HIT_MONSTER);
						common::audio::PlaySound(application::Sounds::HIT_MONSTER);
					}
				}
				else
				{
					SetCombatResultText(MISSED_MONSTER);
					common::audio::PlaySound(application::Sounds::HIT_BLOCKED);
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
					SetCombatResultText(GOT_HIT);
					common::audio::PlaySound(application::Sounds::HIT_HUNTER);
					game::avatar::Statistics::Decrease(game::avatar::Statistic::HEALTH, damage);
				}
				else
				{
					SetCombatResultText(BLOCKED_HIT);
					common::audio::PlaySound(application::Sounds::HIT_BLOCKED);
				}
				DoDefendTimer();
			}
		}
		else
		{
			game::avatar::Statistics::Decrease(game::avatar::Statistic::HEALTH, game::Creatures::GetAttack(game::Avatar::GetPosition()).value());
			DoDefendTimer();
		}
	}
}