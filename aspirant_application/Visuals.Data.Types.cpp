#include "Visuals.Data.Types.h"
#include <map>
namespace visuals::data::Types
{
	const std::string AVATAR_INVENTORY = "AvatarInventory";
	const std::string IMAGE = "Image";
	const std::string LAYOUT = "Layout";
	const std::string MENU = "Menu";
	const std::string TEXT = "Text";
	const std::string WORLD_MAP = "WorldMap";
	const std::string AREA = "Area";
}
namespace visuals::data
{
	const std::map<std::string, Type> table =
	{
		{ visuals::data::Types::AVATAR_INVENTORY, Type::AVATAR_INVENTORY },
		{ visuals::data::Types::IMAGE, Type::IMAGE },
		{ visuals::data::Types::LAYOUT, Type::LAYOUT },
		{ visuals::data::Types::MENU, Type::MENU },
		{ visuals::data::Types::TEXT, Type::TEXT },
		{ visuals::data::Types::WORLD_MAP, Type::WORLD_MAP },
		{ visuals::data::Types::AREA, Type::AREA }
	};
}
namespace visuals::data::Types
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
