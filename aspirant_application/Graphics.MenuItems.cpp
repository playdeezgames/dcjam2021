#include "Graphics.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Types.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
namespace graphics::Layouts
{
	extern std::map<std::string, nlohmann::json> layouts;
}
namespace graphics::MenuItems
{
	void SetItemText(const std::string& layoutName, const std::string& menuItemId, const std::string& text)
	{
		for (auto& thingie : graphics::Layouts::layouts[layoutName])
		{
			if (thingie[common::Properties::TYPE] == graphics::Types::MENU)
			{
				for (auto& menuItem : thingie[graphics::Properties::MENU_ITEMS])
				{
					if (menuItem.count(graphics::Properties::MENU_ITEM_ID) > 0 &&
						menuItem[graphics::Properties::MENU_ITEM_ID] == menuItemId)
					{
						menuItem[graphics::Properties::TEXT] = text;
					}
				}
			}
		}
	}
}