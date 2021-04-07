#pragma once
#include <tuple>
namespace common::card
{
	enum class Rank
	{
		DEUCE,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
		TEN,
		JACK,
		QUEEN,
		KING,
		ACE
	};

	enum class Suit
	{
		CLUB,
		DIAMOND,
		HEART,
		SPADE
	};
}
namespace common
{
	typedef std::tuple<card::Rank, card::Suit> Card;
}
