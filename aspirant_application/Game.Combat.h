#pragma once
#include <optional>
namespace game::Combat
{
	void Advance();
	enum class Guess
	{
		HIGHER,
		LOWER
	};
	enum class CombatResult
	{
		MONSTER_KILLED,
		MONSTER_HIT,
		MONSTER_BLOCKED,
		HUNTER_HIT,
		HUNTER_BLOCKED,
		HUNTER_RAN
	};
	CombatResult Resolve(std::optional<Guess>);
}
