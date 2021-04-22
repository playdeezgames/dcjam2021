#include "Graphics.Data.Types.h"
#include <map>
namespace graphics::data::Types
{
	const std::string AVATAR_INVENTORY = "AvatarInventory";
	const std::string FLOOR_INVENTORY = "FloorInventory";
	const std::string IMAGE = "Image";
	const std::string LAYOUT = "Layout";
	const std::string MENU = "Menu";
	const std::string TEXT = "Text";
	const std::string WORLD_MAP = "WorldMap";
	const std::string AREA = "Area";
}
namespace graphics::data
{
	const std::map<std::string, Type> table =
	{
		{ graphics::data::Types::AVATAR_INVENTORY, Type::AVATAR_INVENTORY },
		{ graphics::data::Types::FLOOR_INVENTORY, Type::FLOOR_INVENTORY },
		{ graphics::data::Types::IMAGE, Type::IMAGE },
		{ graphics::data::Types::LAYOUT, Type::LAYOUT },
		{ graphics::data::Types::MENU, Type::MENU },
		{ graphics::data::Types::TEXT, Type::TEXT },
		{ graphics::data::Types::WORLD_MAP, Type::WORLD_MAP },
		{ graphics::data::Types::AREA, Type::AREA }
	};
}
namespace graphics::data::Types
{
	std::optional<Type> FromString(const std::string& name)
	{
		auto iter = table.find(name);
		if (iter != table.end())
		{
			return iter->second;
		}
		return std::nullopt;
	}
}
