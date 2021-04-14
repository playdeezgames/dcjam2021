#include "Game.CombatDeck.h"
#include <vector>
#include "Common.RNG.h"
#include <algorithm>
#include <string>
#include <map>
#include "Common.Deck.h"
namespace game::CombatDeck
{
	static common::Deck<common::Card> deck
	({
		{common::card::Rank::DEUCE, common::card::Suit::CLUB},
		{common::card::Rank::DEUCE, common::card::Suit::DIAMOND},
		{common::card::Rank::DEUCE, common::card::Suit::HEART},
		{common::card::Rank::DEUCE, common::card::Suit::SPADE},
		{common::card::Rank::THREE, common::card::Suit::CLUB},
		{common::card::Rank::THREE, common::card::Suit::DIAMOND},
		{common::card::Rank::THREE, common::card::Suit::HEART},
		{common::card::Rank::THREE, common::card::Suit::SPADE},
		{common::card::Rank::FOUR, common::card::Suit::CLUB},
		{common::card::Rank::FOUR, common::card::Suit::DIAMOND},
		{common::card::Rank::FOUR, common::card::Suit::HEART},
		{common::card::Rank::FOUR, common::card::Suit::SPADE},
		{common::card::Rank::FIVE, common::card::Suit::CLUB},
		{common::card::Rank::FIVE, common::card::Suit::DIAMOND},
		{common::card::Rank::FIVE, common::card::Suit::HEART},
		{common::card::Rank::FIVE, common::card::Suit::SPADE},
		{common::card::Rank::SIX, common::card::Suit::CLUB},
		{common::card::Rank::SIX, common::card::Suit::DIAMOND},
		{common::card::Rank::SIX, common::card::Suit::HEART},
		{common::card::Rank::SIX, common::card::Suit::SPADE},
		{common::card::Rank::SEVEN, common::card::Suit::CLUB},
		{common::card::Rank::SEVEN, common::card::Suit::DIAMOND},
		{common::card::Rank::SEVEN, common::card::Suit::HEART},
		{common::card::Rank::SEVEN, common::card::Suit::SPADE},
		{common::card::Rank::EIGHT, common::card::Suit::CLUB},
		{common::card::Rank::EIGHT, common::card::Suit::DIAMOND},
		{common::card::Rank::EIGHT, common::card::Suit::HEART},
		{common::card::Rank::EIGHT, common::card::Suit::SPADE},
		{common::card::Rank::NINE, common::card::Suit::CLUB},
		{common::card::Rank::NINE, common::card::Suit::DIAMOND},
		{common::card::Rank::NINE, common::card::Suit::HEART},
		{common::card::Rank::NINE, common::card::Suit::SPADE},
		{common::card::Rank::TEN, common::card::Suit::CLUB},
		{common::card::Rank::TEN, common::card::Suit::DIAMOND},
		{common::card::Rank::TEN, common::card::Suit::HEART},
		{common::card::Rank::TEN, common::card::Suit::SPADE},
		{common::card::Rank::JACK, common::card::Suit::CLUB},
		{common::card::Rank::JACK, common::card::Suit::DIAMOND},
		{common::card::Rank::JACK, common::card::Suit::HEART},
		{common::card::Rank::JACK, common::card::Suit::SPADE},
		{common::card::Rank::QUEEN, common::card::Suit::CLUB},
		{common::card::Rank::QUEEN, common::card::Suit::DIAMOND},
		{common::card::Rank::QUEEN, common::card::Suit::HEART},
		{common::card::Rank::QUEEN, common::card::Suit::SPADE},
		{common::card::Rank::KING, common::card::Suit::CLUB},
		{common::card::Rank::KING, common::card::Suit::DIAMOND},
		{common::card::Rank::KING, common::card::Suit::HEART},
		{common::card::Rank::KING, common::card::Suit::SPADE},
		{common::card::Rank::ACE, common::card::Suit::CLUB},
		{common::card::Rank::ACE, common::card::Suit::DIAMOND},
		{common::card::Rank::ACE, common::card::Suit::HEART},
		{common::card::Rank::ACE, common::card::Suit::SPADE}
	});

	static common::Card currentCard;
	static common::Card nextCard;

	const common::Card& GetCurrentCard()
	{
		return currentCard;
	}

	const common::Card& GetNextCard()
	{
		return nextCard;
	}

	void Deal()
	{
		currentCard = nextCard;
		if (!deck.Peek())
		{
			deck.Shuffle();
		}
		nextCard = deck.Draw().value();
	}

	bool IsGuessCorrect(Guess guess)
	{
		switch (guess)
		{
		case Guess::HIGHER:
			return std::get<0>(GetNextCard()) > std::get<0>(GetCurrentCard());
		case Guess::LOWER:
			return std::get<0>(GetNextCard()) < std::get<0>(GetCurrentCard());
		}
		return false;
	}
}
