#include "Graphics.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Graphics.Data.Properties.h"
#include "Graphics.MenuItems.h"
namespace graphics::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace graphics::MenuItems
{
	template <typename TResult>
	static TResult WithMenuItem(const std::string& layoutName, const std::string& menuItemId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : graphics::Layouts::GetLayout(layoutName))
		{
			if (graphics::data::Types::FromString(thingie[common::data::Properties::TYPE]) == graphics::data::Type::MENU)
			{
				for (auto& menuItem : thingie[graphics::data::Properties::MENU_ITEMS])
				{
					if (menuItem.count(graphics::data::Properties::MENU_ITEM_ID) > 0 &&
						menuItem[graphics::data::Properties::MENU_ITEM_ID] == menuItemId)
					{
						return func(menuItem);
					}
				}
			}
		}
		return notFound();
	}

	void SetText(const std::string& layoutName, const std::string& menuItemId, const std::string& text)
	{
		WithMenuItem<void>(layoutName, menuItemId, [text](auto& menuItem)
		{
			menuItem[graphics::data::Properties::TEXT] = text;
		}, []() {});
	}
}