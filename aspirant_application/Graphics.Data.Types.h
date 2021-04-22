#pragma once
#include <string>
#include <optional>
namespace graphics::data
{
	enum class Type
	{
		AVATAR_INVENTORY,
		FLOOR_INVENTORY,
		IMAGE,
		LAYOUT,
		MENU,
		TEXT,
		WORLD_MAP,
		AREA
	};
}
namespace graphics::data::Types
{
	std::optional<graphics::data::Type> FromString(const std::string&);
}
