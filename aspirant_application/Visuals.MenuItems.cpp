#include "Visuals.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.MenuItems.h"
namespace visuals::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace visuals::MenuItems
{
	template <typename TResult>
	static TResult WithMenuItem(const std::string& layoutName, const std::string& menuItemId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : visuals::Layouts::GetLayout(layoutName))
		{
			if (visuals::data::Types::FromString(thingie[common::data::Properties::TYPE]) == visuals::data::Type::MENU)
			{
				for (auto& menuItem : thingie[visuals::data::Properties::MENU_ITEMS])
				{
					if (menuItem.count(visuals::data::Properties::MENU_ITEM_ID) > 0 &&
						menuItem[visuals::data::Properties::MENU_ITEM_ID] == menuItemId)
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
			menuItem[visuals::data::Properties::TEXT] = text;
		}, []() {});
	}
}