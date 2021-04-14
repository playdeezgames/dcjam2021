#include "Game.Combat.h"
#include "Game.CombatDeck.h"
#include "Game.Avatar.h"
#include "Game.Creatures.h"
#include "Game.Avatar.Statistics.h"
#include "Application.Sounds.h"
#include "Common.Audio.h"
#include "Graphics.Texts.h"
#include <tuple>
namespace game::Combat
{
	//TODO: move to State.InPlay.Combat vvv
	const std::string LAYOUT_NAME = "State.InPlay.CombatResult";
	const std::string RESULT_TEXT_ID = "Result";
	const std::string HIT_MONSTER = "You hit it!";
	const std::string KILL_MONSTER = "You killed it!";
	const std::string GOT_HIT = "It hit you!";
	const std::string BLOCKED_HIT = "You block!";
	const std::string MISSED_MONSTER = "It blocked!";
	const std::string HUNTER_RAN = "You attempted to run!";//Doesnt matter... never seen

	void SetCombatResultText(const std::string& text)//TODO: move to State.InPlay.Combat
	{
		graphics::Texts::SetText(LAYOUT_NAME, RESULT_TEXT_ID, text);
	}

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

	enum class CombatResult
	{
		MONSTER_KILLED,
		MONSTER_HIT,
		MONSTER_BLOCKED,
		HUNTER_HIT,
		HUNTER_BLOCKED,
		HUNTER_RAN
	};

	static CombatResult DetermineResult(std::optional<Guess> guess)//TODO: badly named
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

	const std::map<CombatResult, std::tuple<std::string, std::string>> resolutions =
	{
		{CombatResult::MONSTER_KILLED, {KILL_MONSTER, application::Sounds::DEAD_MONSTER}},
		{CombatResult::MONSTER_HIT, {HIT_MONSTER, application::Sounds::HIT_MONSTER}},
		{CombatResult::MONSTER_BLOCKED, {MISSED_MONSTER, application::Sounds::HIT_BLOCKED}},
		{CombatResult::HUNTER_HIT, {GOT_HIT, application::Sounds::HIT_HUNTER}},
		{CombatResult::HUNTER_BLOCKED, {BLOCKED_HIT, application::Sounds::HIT_BLOCKED}},
		{CombatResult::HUNTER_RAN, {HUNTER_RAN, application::Sounds::HIT_HUNTER}}
	};

	void Resolve(std::optional<Guess> guess)//TODO: this returns the tuple, and the text updated and the sound playing moves to State.InPlay.Combat
	{
		auto& resolutionDetails = resolutions.find(DetermineResult(guess))->second;
		SetCombatResultText(std::get<0>(resolutionDetails));//TODO: side effect
		common::audio::Sfx::Play(std::get<1>(resolutionDetails));//TODO: side effect
	}
}