#pragma once
#include <string>
namespace game::Combat
{
	struct CardDescriptor
	{
		size_t rank;
		std::string sprite;
	};

	void Shuffle();
	const CardDescriptor& GetCurrentCard();
	const CardDescriptor& GetNextCard();
	void Advance();
}
#include <optional>
namespace game::Combat
{
	enum class Guess
	{
		HIGHER,
		LOWER
	};
	void Resolve(std::optional<Guess>);
}
