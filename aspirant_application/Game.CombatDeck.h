#pragma once
#include <string>
#include "Common.Card.h"
namespace game::CombatDeck
{
	enum class Guess
	{
		HIGHER,
		LOWER
	};
	const common::Card& GetCurrentCard();
	const common::Card& GetNextCard();
	void Deal();
	bool IsGuessCorrect(Guess);
}
