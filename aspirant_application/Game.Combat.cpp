#include "Game.Combat.h"
#include <vector>
#include "Common.RNG.h"
#include <algorithm>
#include <string>
namespace game::Combat
{
	const size_t DECK_SIZE = 52;

	static std::vector<size_t> deck;
	static size_t deckIndex = 0;

	const std::vector<CardDescriptor> cardDescriptors =
	{
		{0, "Card2C" },
		{0, "Card2D" },
		{0, "Card2H" },
		{0, "Card2S" },
		{1, "Card3C" },
		{1, "Card3D" },
		{1, "Card3H" },
		{1, "Card3S" },
		{2, "Card4C" },
		{2, "Card4D" },
		{2, "Card4H" },
		{2, "Card4S" },
		{3, "Card5C" },
		{3, "Card5D" },
		{3, "Card5H" },
		{3, "Card5S" },
		{4, "Card6C" },
		{4, "Card6D" },
		{4, "Card6H" },
		{4, "Card6S" },
		{5, "Card7C" },
		{5, "Card7D" },
		{5, "Card7H" },
		{5, "Card7S" },
		{6, "Card8C" },
		{6, "Card8D" },
		{6, "Card8H" },
		{6, "Card8S" },
		{7, "Card9C" },
		{7, "Card9D" },
		{7, "Card9H" },
		{7, "Card9S" },
		{8, "Card10C" },
		{8, "Card10D" },
		{8, "Card10H" },
		{8, "Card10S" },
		{9, "CardJC" },
		{9, "CardJD" },
		{9, "CardJH" },
		{9, "CardJS" },
		{10, "CardQC" },
		{10, "CardQD" },
		{10, "CardQH" },
		{10, "CardQS" },
		{11, "CardKC" },
		{11, "CardKD" },
		{11, "CardKH" },
		{11, "CardKS" },
		{12, "CardAC" },
		{12, "CardAD" },
		{12, "CardAH" },
		{12, "CardAS" }
	};

	const CardDescriptor& GetCurrentCard()
	{
		return cardDescriptors[deck[deckIndex]];
	}

	const CardDescriptor& GetNextCard()
	{
		return cardDescriptors[deck[deckIndex + 1]];
	}

	void Shuffle()
	{
		deck.clear();
		while (deck.size() < DECK_SIZE)
		{
			deck.push_back(deck.size());
		}
		for (size_t index = 0; index < DECK_SIZE; ++index)
		{
			size_t otherIndex = (size_t)common::RNG::FromRange(0, (int)DECK_SIZE);
			size_t dummy = deck[index];
			deck[index] = deck[otherIndex];
			deck[otherIndex] = dummy;
		}
		deckIndex = 0;
	}

}
#include "Game.Avatar.h"
#include "Game.Creatures.h"
#include "Game.Avatar.Statistics.h"
#include "Application.Sounds.h"
#include "Common.Sounds.h"
namespace game::Combat
{
	void Advance()
	{
		game::Creatures::Advance(game::Avatar::GetPosition());
		deckIndex++;
		if (deckIndex >= DECK_SIZE / 2)
		{
			Shuffle();
		}
	}

	static bool IsGuessCorrect(Guess guess)
	{
		switch (guess)
		{
		case Guess::HIGHER:
			return GetNextCard().rank > GetCurrentCard().rank;
		case Guess::LOWER:
			return GetNextCard().rank < GetCurrentCard().rank;
		}
		return false;
	}

	void Resolve(std::optional<Guess> guess)
	{
		if (guess)
		{
			if (IsGuessCorrect(guess.value()))
			{
				game::Creatures::DecreaseHealth(game::Avatar::GetPosition(), game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK));
				if (game::Creatures::IsDead(game::Avatar::GetPosition()).value())
				{
					common::Sounds::PlaySound(application::Sounds::DEAD_MONSTER);
				}
				else
				{
					common::Sounds::PlaySound(application::Sounds::HIT_MONSTER);
				}
			}
			else
			{
				common::Sounds::PlaySound(application::Sounds::HIT_HUNTER);
				game::avatar::Statistics::Decrease(game::avatar::Statistic::HEALTH, game::Creatures::GetAttack(game::Avatar::GetPosition()).value());
			}
		}
		else
		{
			game::avatar::Statistics::Decrease(game::avatar::Statistic::HEALTH, game::Creatures::GetAttack(game::Avatar::GetPosition()).value());
		}
	}
}