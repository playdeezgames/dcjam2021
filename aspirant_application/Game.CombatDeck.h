#pragma once
#include <string>
#include "Common.Card.h"
namespace game::CombatDeck
{
	const common::Card& GetCurrentCard();
	const common::Card& GetNextCard();
	void Deal();
	const std::string& GetSpriteForCard(const common::Card&);
}
