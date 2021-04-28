#pragma once
#include <string>
#include <optional>
namespace application::UI
{
	enum class Sfx
	{
		BUMP_WALL,
		DEAD_HUNTER,
		EXIT,
		HIT_HUNTER,
		HIT_BLOCKED,
		WOOHOO,
		HUNTER_POOPS
	};
}

namespace application::Sounds
{
	std::optional<std::string> Read(const application::UI::Sfx&);
}
