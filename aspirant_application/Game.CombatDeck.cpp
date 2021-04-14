#include "Game.CombatDeck.h"
#include <vector>
#include "Common.RNG.h"
#include <algorithm>
#include <string>
#include <map>
#include "Common.Deck.h"
namespace game::CombatDeck
{
	const std::map<common::Card, std::string> cardSprites =
	{
		{{common::card::Rank::DEUCE, common::card::Suit::CLUB}, "Card2C" },
		{{common::card::Rank::DEUCE, common::card::Suit::DIAMOND}, "Card2D" },
		{{common::card::Rank::DEUCE, common::card::Suit::HEART}, "Card2H" },
		{{common::card::Rank::DEUCE, common::card::Suit::SPADE}, "Card2S" },
		{{common::card::Rank::THREE, common::card::Suit::CLUB}, "Card3C" },
		{{common::card::Rank::THREE, common::card::Suit::DIAMOND}, "Card3D" },
		{{common::card::Rank::THREE, common::card::Suit::HEART}, "Card3H" },
		{{common::card::Rank::THREE, common::card::Suit::SPADE}, "Card3S" },
		{{common::card::Rank::FOUR, common::card::Suit::CLUB}, "Card4C" },
		{{common::card::Rank::FOUR, common::card::Suit::DIAMOND}, "Card4D" },
		{{common::card::Rank::FOUR, common::card::Suit::HEART}, "Card4H" },
		{{common::card::Rank::FOUR, common::card::Suit::SPADE}, "Card4S" },
		{{common::card::Rank::FIVE, common::card::Suit::CLUB}, "Card5C" },
		{{common::card::Rank::FIVE, common::card::Suit::DIAMOND}, "Card5D" },
		{{common::card::Rank::FIVE, common::card::Suit::HEART}, "Card5H" },
		{{common::card::Rank::FIVE, common::card::Suit::SPADE}, "Card5S" },
		{{common::card::Rank::SIX, common::card::Suit::CLUB}, "Card6C" },
		{{common::card::Rank::SIX, common::card::Suit::DIAMOND}, "Card6D" },
		{{common::card::Rank::SIX, common::card::Suit::HEART}, "Card6H" },
		{{common::card::Rank::SIX, common::card::Suit::SPADE}, "Card6S" },
		{{common::card::Rank::SEVEN, common::card::Suit::CLUB}, "Card7C" },
		{{common::card::Rank::SEVEN, common::card::Suit::DIAMOND}, "Card7D" },
		{{common::card::Rank::SEVEN, common::card::Suit::HEART}, "Card7H" },
		{{common::card::Rank::SEVEN, common::card::Suit::SPADE}, "Card7S" },
		{{common::card::Rank::EIGHT, common::card::Suit::CLUB}, "Card8C" },
		{{common::card::Rank::EIGHT, common::card::Suit::DIAMOND}, "Card8D" },
		{{common::card::Rank::EIGHT, common::card::Suit::HEART}, "Card8H" },
		{{common::card::Rank::EIGHT, common::card::Suit::SPADE}, "Card8S" },
		{{common::card::Rank::NINE, common::card::Suit::CLUB}, "Card9C" },
		{{common::card::Rank::NINE, common::card::Suit::DIAMOND}, "Card9D" },
		{{common::card::Rank::NINE, common::card::Suit::HEART}, "Card9H" },
		{{common::card::Rank::NINE, common::card::Suit::SPADE}, "Card9S" },
		{{common::card::Rank::TEN, common::card::Suit::CLUB}, "Card10C" },
		{{common::card::Rank::TEN, common::card::Suit::DIAMOND}, "Card10D" },
		{{common::card::Rank::TEN, common::card::Suit::HEART}, "Card10H" },
		{{common::card::Rank::TEN, common::card::Suit::SPADE}, "Card10S" },
		{{common::card::Rank::JACK, common::card::Suit::CLUB}, "CardJC" },
		{{common::card::Rank::JACK, common::card::Suit::DIAMOND}, "CardJD" },
		{{common::card::Rank::JACK, common::card::Suit::HEART}, "CardJH" },
		{{common::card::Rank::JACK, common::card::Suit::SPADE}, "CardJS" },
		{{common::card::Rank::QUEEN, common::card::Suit::CLUB}, "CardQC" },
		{{common::card::Rank::QUEEN, common::card::Suit::DIAMOND}, "CardQD" },
		{{common::card::Rank::QUEEN, common::card::Suit::HEART}, "CardQH" },
		{{common::card::Rank::QUEEN, common::card::Suit::SPADE}, "CardQS" },
		{{common::card::Rank::KING, common::card::Suit::CLUB}, "CardKC" },
		{{common::card::Rank::KING, common::card::Suit::DIAMOND}, "CardKD" },
		{{common::card::Rank::KING, common::card::Suit::HEART}, "CardKH" },
		{{common::card::Rank::KING, common::card::Suit::SPADE}, "CardKS" },
		{{common::card::Rank::ACE, common::card::Suit::CLUB}, "CardAC" },
		{{common::card::Rank::ACE, common::card::Suit::DIAMOND}, "CardAD" },
		{{common::card::Rank::ACE, common::card::Suit::HEART}, "CardAH" },
		{{common::card::Rank::ACE, common::card::Suit::SPADE}, "CardAS" }
	};

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

	const std::string& GetSpriteForCard(const common::Card& card)
	{
		return cardSprites.find(card)->second;
	}

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
