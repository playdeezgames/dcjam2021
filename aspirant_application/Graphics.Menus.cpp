#include "Graphics.Menus.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Types.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
namespace graphics::Layouts 
{ 
	extern std::map<std::string, nlohmann::json> layouts;
}
namespace graphics::Menus
{
	std::optional<int> Read(const std::string& layoutName, const std::string& menuId)
	{
		for (auto& thingie : graphics::Layouts::layouts[layoutName])
		{
			if (thingie[common::Properties::TYPE] == graphics::Types::MENU &&
				thingie[graphics::Properties::MENU_ID] == menuId)
			{
				int index = thingie[graphics::Properties::INDEX];//TODO: this cannot be proxied!
				return (int)thingie[graphics::Properties::MENU_ITEMS][index][graphics::Properties::VALUE];
			}
		}
		return std::nullopt;
	}

	static void ChangeMenuIndex(const std::string& layoutName, const std::string& menuId, int delta)
	{
		for (auto& thingie : graphics::Layouts::layouts[layoutName])
		{
			if (thingie[common::Properties::TYPE] == graphics::Types::MENU &&
				thingie[graphics::Properties::MENU_ID] == menuId)
			{
				auto itemCount = thingie[graphics::Properties::MENU_ITEMS].size();
				//TODO: this cannot be proxied! vv
				thingie[graphics::Properties::INDEX] = (thingie[graphics::Properties::INDEX] + itemCount + delta) % itemCount;
			}
		}
	}

	void Next(const std::string& layoutName, const std::string& menuId)
	{
		ChangeMenuIndex(layoutName, menuId, 1);
	}

	void Previous(const std::string& layoutName, const std::string& menuId)
	{
		ChangeMenuIndex(layoutName, menuId, -1);
	}
}