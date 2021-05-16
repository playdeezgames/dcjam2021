#include "Visuals.Menus.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Layouts.h"
#include "Common.XY.h"
#include "Visuals.Fonts.h"
namespace visuals::Menu
{
	struct InternalMenuItem
	{
		std::string text;
		common::XY<int> xy;
		bool enabled;
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
	static std::map<std::string, std::map<std::string, size_t>> menuItemTable;

	static void DrawInternalMenu(const std::shared_ptr<SDL_Renderer>& renderer, size_t menuIndex)
	{
		auto& menu = internalMenus[menuIndex];
		int currentIndex = 0;
		for (auto& menuItemIndex : menu.menuItems)
		{
			auto& menuItem = internalMenuItems[menuItemIndex];
			if (menuItem.enabled)
			{
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
			}
			currentIndex++;
		}
	}

	static size_t InternalizeMenuItem(const std::string& layoutName, const nlohmann::json& menuItem)
	{
		size_t menuItemIndex = internalMenuItems.size();
		if (menuItem.count(visuals::data::Properties::MENU_ITEM_ID) > 0)
		{
			menuItemTable[layoutName][menuItem[visuals::data::Properties::MENU_ITEM_ID]] = menuItemIndex;
		}
		bool enabled = (menuItem.count(visuals::data::Properties::ENABLED) > 0) ? ((bool)menuItem[visuals::data::Properties::ENABLED]) : (true);
		internalMenuItems.push_back(
			{
				menuItem[data::Properties::TEXT],
				common::XY<int>(
					menuItem[common::data::Properties::X],
					menuItem[common::data::Properties::Y]),
				enabled
			});
		return menuItemIndex;
	}

	std::function<void(const std::shared_ptr<SDL_Renderer>&)> Internalize(const std::string& layoutName, const nlohmann::json& model)
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
			internalMenu.menuItems.push_back(InternalizeMenuItem(layoutName, menuItem));
		}
		internalMenus.push_back(internalMenu);
		if (model.count(visuals::data::Properties::MENU_ID) > 0)
		{
			menuTable[layoutName][model[visuals::data::Properties::MENU_ID]] = menuIndex;
		}
		return [menuIndex](const std::shared_ptr<SDL_Renderer>& renderer) {
			DrawInternalMenu(renderer, menuIndex);
		};
	}

	static const size_t GetMenuIndex(const std::string& layoutName, const std::string& menuId)
	{
		return menuTable.find(layoutName)->second.find(menuId)->second;
	}

	static const InternalMenu& GetMenu(const std::string& layoutName, const std::string& menuId)
	{
		return internalMenus[GetMenuIndex(layoutName, menuId)];
	}

	static size_t GetMenuItemIndex(const std::string& layoutName, const std::string& menuItemId)
	{
		return menuItemTable.find(layoutName)->second.find(menuItemId)->second;
	}
}
namespace visuals::Menus
{

	std::optional<int> ReadIndex(const std::string& layoutName, const std::string& menuId)
	{
		return Menu::GetMenu(layoutName, menuId).index;
	}

	enum class WriteConditional
	{
		CHECK,
		FORCE
	};

	static void WriteIndex(const std::string& layoutName, const std::string& menuId, int index, WriteConditional conditional)
	{
		auto menuIndex = Menu::GetMenuIndex(layoutName, menuId);
		size_t menuItemId = visuals::Menu::internalMenus[menuIndex].menuItems[index];
		if ((conditional==WriteConditional::FORCE) || visuals::Menu::internalMenuItems[menuItemId].enabled)
		{
			visuals::Menu::internalMenus[menuIndex].index = index;
		}
	}

	void WriteIndex(const std::string& layoutName, const std::string& menuId, int index)
	{
		WriteIndex(layoutName, menuId, index, WriteConditional::CHECK);
	}

	size_t GetCount(const std::string& layoutName, const std::string& menuId)
	{
		return Menu::GetMenu(layoutName, menuId).menuItems.size();
	}

	static bool HasEnabledIndices(const std::string& layoutName, const std::string& menuId)
	{
		auto& menu = Menu::GetMenu(layoutName, menuId);
		auto& menuItems = menu.menuItems;
		for (auto menuItem : menuItems)
		{
			if (visuals::Menu::internalMenuItems[menuItem].enabled)
			{
				return true;
			}
		}
		return false;
	}

	static bool IsEnabledIndex(const std::string& layoutName, const std::string& menuId, size_t index)
	{
		if (index < GetCount(layoutName, menuId))
		{
			auto& menu = Menu::GetMenu(layoutName, menuId);
			auto& menuItem = menu.menuItems[index];
			return visuals::Menu::internalMenuItems[menuItem].enabled;
		}
		return false;
	}

	enum class Change
	{
		NEXT,
		PREVIOUS
	};

	const std::map<Change, int> changeDeltas =
	{
		{Change::NEXT, 1},
		{Change::PREVIOUS, -1}
	};

	static void ChangeMenuIndex(const std::string& layoutName, const std::string& menuId, Change change)
	{
		if (HasEnabledIndices(layoutName, menuId))
		{
			auto itemCount = GetCount(layoutName, menuId);
			int delta = changeDeltas.find(change)->second;
			do
			{
				auto index = ReadIndex(layoutName, menuId).value();
				auto nextIndex = ((size_t)index + (int)itemCount + delta) % itemCount;
				WriteIndex(layoutName, menuId, (int)nextIndex, WriteConditional::FORCE);
			} while (!IsEnabledIndex(layoutName, menuId, *ReadIndex(layoutName, menuId)));
		}
		else 
		{
			WriteIndex(layoutName, menuId, 0, WriteConditional::FORCE);
		}
	}

	std::function<void()> NavigateNext(const std::string& layoutName, const std::string& menuId)
	{
		return [layoutName, menuId]() 
		{
			ChangeMenuIndex(layoutName, menuId, Change::NEXT);
		};
	}

	std::function<void()> NavigatePrevious(const std::string& layoutName, const std::string& menuId)
	{
		return [layoutName, menuId]()
		{
			ChangeMenuIndex(layoutName, menuId, Change::PREVIOUS);
		};
	}
}
namespace visuals::MenuItems
{
	void SetText(const std::string& layoutName, const std::string& menuItemId, const std::string& text)
	{
		auto menuItemIndex = Menu::GetMenuItemIndex(layoutName, menuItemId);
		visuals::Menu::internalMenuItems[menuItemIndex].text = text;
	}
	void SetEnabled(const std::string& layoutName, const std::string& menuItemId, bool enabled)
	{
		auto menuItemIndex = Menu::GetMenuItemIndex(layoutName, menuItemId);
		visuals::Menu::internalMenuItems[menuItemIndex].enabled = enabled;
	}
}