#pragma once
#include <vector>
#include <optional>
#include <algorithm>
#include "Common.RNG.h"
namespace common
{
	template<typename TCard>
	class Deck
	{
	private:
		std::vector<TCard> cards;
		size_t index;
	public:
		Deck(const std::vector<TCard>& cards)
			: cards(cards)
			, index(cards.size())
		{

		}

		void Shuffle()
		{
			index = 0;
			for (int first = 0; first < cards.size(); ++first)
			{
				int second = common::RNG::FromRange(0, (int)cards.size());
				std::swap(cards[first], cards[second]);
			}
		}

		std::optional<TCard> Peek()
		{
			if (index < cards.size())
			{
				return cards[index];
			}
			return std::nullopt;
		}

		std::optional<TCard> Draw()
		{
			auto result = Peek();
			if (result)
			{
				index++;
			}
			return result;
		}
	};
}
