#include "Visuals.Menus.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Layouts.h"
#include "Common.XY.h"
#include "Visuals.Fonts.h"
namespace visuals::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace visuals::Menu
{
	struct InternalMenuItem
	{
		std::string text;
		common::XY<int> xy;
		int value;
	};

	struct InternalMenu
	{
		int index;
		std::string font;
		std::string activeColor;
		std::string inactiveColor;
		visuals::HorizontalAlignment alignment;
		bool dropShadow;
		common::XY<int> dropShadowXY;
		std::string dropShadowColor;
		std::vector<size_t> menuItems;
	};

	static std::vector<InternalMenuItem> internalMenuItems;
	static std::vector<InternalMenu> internalMenus;
	static std::map<std::string, std::map<std::string, size_t>> menuTable;

	static void DrawInternalMenu(std::shared_ptr<SDL_Renderer> renderer, size_t menuIndex)
	{
		auto& menu = internalMenus[menuIndex];
		int currentIndex = 0;
		for (auto& menuItemIndex : menu.menuItems)
		{
			auto& menuItem = internalMenuItems[menuItemIndex];
			if (menu.dropShadow)
			{
				Fonts::WriteText(
					menu.font,
					renderer,
					menu.dropShadowXY + menuItem.xy,
					menuItem.text,
					menu.dropShadowColor,
					menu.alignment);
			}
			std::string color = (currentIndex == menu.index) ? (menu.activeColor) : (menu.inactiveColor);
			Fonts::WriteText(menu.font, renderer, menuItem.xy, menuItem.text, color, menu.alignment);
			currentIndex++;
		}
	}

	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		size_t menuIndex = internalMenus.size();
		InternalMenu internalMenu = {
			model[data::Properties::INDEX],
			model[data::Properties::FONT],
			model[data::Properties::COLORS][data::Properties::ACTIVE],
			model[data::Properties::COLORS][data::Properties::INACTIVE],
			(visuals::HorizontalAlignment)(int)model[data::Properties::HORIZONTAL_ALIGNMENT],
			model[data::Properties::DROP_SHADOW],
			common::XY<int>(
				model[data::Properties::DROP_SHADOW_X],
				model[data::Properties::DROP_SHADOW_Y]),
			model[data::Properties::DROP_SHADOW_COLOR],
			std::vector<size_t>()
		};
		auto& menuItems = model[data::Properties::MENU_ITEMS];
		for (auto& menuItem : menuItems)
		{
			size_t menuItemIndex = internalMenuItems.size();
			internalMenu.menuItems.push_back(menuItemIndex);
			internalMenuItems.push_back(
				{ 
					menuItem[data::Properties::TEXT],
					common::XY<int>(
						menuItem[common::data::Properties::X],
						menuItem[common::data::Properties::Y]),
					menuItem[visuals::data::Properties::VALUE]
				});
		}
		internalMenus.push_back(internalMenu);
		if (model.count(visuals::data::Properties::MENU_ID) > 0)
		{
			menuTable[layoutName][model[visuals::data::Properties::MENU_ID]] = menuIndex;
		}
		return [menuIndex](std::shared_ptr<SDL_Renderer> renderer) {
			DrawInternalMenu(renderer, menuIndex);
		};
	}
}
namespace visuals::Menus
{
	std::optional<int> ReadIndex(const std::string& layoutName, const std::string& menuId)
	{
		return visuals::Menu::internalMenus[visuals::Menu::menuTable.find(layoutName)->second.find(menuId)->second].index;
	}

	std::optional<int> ReadValue(const std::string& layoutName, const std::string& menuId)
	{
		auto& menu = visuals::Menu::internalMenus[visuals::Menu::menuTable.find(layoutName)->second.find(menuId)->second];
		auto& menuItem = visuals::Menu::internalMenuItems[menu.menuItems[menu.index]];
		return menuItem.value;
	}

	void WriteIndex(const std::string& layoutName, const std::string& menuId, int index)
	{
		auto menuIndex = visuals::Menu::menuTable.find(layoutName)->second.find(menuId)->second;
		visuals::Menu::internalMenus[menuIndex].index = index;
	}

	size_t GetCount(const std::string& layoutName, const std::string& menuId)
	{
		return visuals::Menu::internalMenus[visuals::Menu::menuTable.find(layoutName)->second.find(menuId)->second].menuItems.size();
	}

	static std::optional<int> FindIndexForValue(const std::string& layoutName, const std::string& menuId, int value)
	{
		auto& menu = visuals::Menu::internalMenus[visuals::Menu::menuTable.find(layoutName)->second.find(menuId)->second];
		auto& menuItems = menu.menuItems;
		int index = 0;
		for (auto menuItemIndex : menuItems)
		{
			if (visuals::Menu::internalMenuItems[menuItemIndex].value == value)
			{
				return index;
			}
			index++;
		}
		return std::nullopt;
	}

	bool WriteValue(const std::string& layoutName, const std::string& menuId, int value)
	{
		auto index = visuals::Menus::FindIndexForValue(layoutName, menuId, value);
		if (index)
		{
			visuals::Menus::WriteIndex(layoutName, menuId, *index);
			return true;
		}
		return false;
	}

	static void ChangeMenuIndex(const std::string& layoutName, const std::string& menuId, int delta)
	{
		auto index = ReadIndex(layoutName, menuId);
		if (index)
		{
			auto itemCount = GetCount(layoutName, menuId);
			if (itemCount > 0)
			{
				WriteIndex(layoutName, menuId, ((*index) + (int)itemCount + delta) % itemCount);
			}
			else
			{
				WriteIndex(layoutName, menuId, 0);
			}
		}
	}

	std::function<void()> NavigateNext(const std::string& layoutName, const std::string& menuId)
	{
		return [layoutName, menuId]() 
		{
			ChangeMenuIndex(layoutName, menuId, 1);
		};
	}

	std::function<void()> NavigatePrevious(const std::string& layoutName, const std::string& menuId)
	{
		return [layoutName, menuId]()
		{
			ChangeMenuIndex(layoutName, menuId, -1);
		};
	}
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