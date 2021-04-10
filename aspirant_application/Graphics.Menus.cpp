#include "Graphics.Menus.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Types.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
namespace graphics::Layouts
{
	//TODO: i dont like this here
	extern std::map<std::string, nlohmann::json> layouts;
}
namespace graphics::Menus
{
	template<typename TResult>
	static TResult WithMenu(const std::string& layoutName, const std::string& menuId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : graphics::Layouts::layouts[layoutName])
		{
			if (thingie[common::Properties::TYPE] == graphics::Types::MENU &&
				thingie[graphics::Properties::MENU_ID] == menuId)
			{
				return func(thingie);
			}
		}
		return notFound();
	}

	std::optional<int> Read(const std::string& layoutName, const std::string& menuId)
	{
		return WithMenu<std::optional<int>>(layoutName, menuId, [](nlohmann::json& thingie)
		{
			int index = thingie[graphics::Properties::INDEX];
			return (int)thingie[graphics::Properties::MENU_ITEMS][index][graphics::Properties::VALUE];
		},
			[]() {return std::nullopt; });
	}

	static void ChangeMenuIndex(const std::string& layoutName, const std::string& menuId, int delta)
	{
		WithMenu<void>(layoutName, menuId, [delta](nlohmann::json& thingie)
		{
			auto itemCount = thingie[graphics::Properties::MENU_ITEMS].size();
			thingie[graphics::Properties::INDEX] = (thingie[graphics::Properties::INDEX] + itemCount + delta) % itemCount;
		},
			[]() {});
	}

	void Next(const std::string& layoutName, const std::string& menuId)
	{
		ChangeMenuIndex(layoutName, menuId, 1);
	}

	void Previous(const std::string& layoutName, const std::string& menuId)
	{
		ChangeMenuIndex(layoutName, menuId, -1);
	}

	size_t GetCount(const std::string& layoutName, const std::string& menuId)
	{
		return WithMenu<size_t>(layoutName, menuId, [](auto& thingie)
		{
			return thingie[graphics::Properties::MENU_ITEMS].size();
		},
			[]() { return 0; });
	}
}