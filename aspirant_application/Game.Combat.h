#pragma once
#include <optional>
#include "Game.CombatDeck.h"
namespace game::Combat
{
	void Advance();
	enum class CombatResult
	{
		MONSTER_KILLED,
		MONSTER_HIT,
		MONSTER_BLOCKED,
		HUNTER_HIT,
		HUNTER_BLOCKED,
		HUNTER_RAN,
		MONSTER_DISTRACTED
	};
	CombatResult Resolve(std::optional<game::CombatDeck::Guess>);
}
