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
	void Resolve(std::optional<Guess>);
}
