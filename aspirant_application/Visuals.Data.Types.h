#pragma once
#include <string>
#include <optional>
namespace visuals::data
{
	enum class Type
	{
		AVATAR_INVENTORY,
		IMAGE,
		LAYOUT,
		MENU,
		TEXT,
		WORLD_MAP,
		AREA
	};
}
namespace visuals::data::Types
{
	std::optional<visuals::data::Type> FromString(const std::string&);
}
