#include "Graphics.Menus.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Types.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
#include "Graphics.Layouts.h"
namespace graphics::Menus
{
	template<typename TResult>
	static TResult WithMenu(const std::string& layoutName, const std::string& menuId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : graphics::Layouts::GetLayout(layoutName))
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

	void Write(const std::string& layoutName, const std::string& menuId, int index)
	{
		return WithMenu<void>(layoutName, menuId, [index](nlohmann::json& thingie)
		{
			thingie[graphics::Properties::INDEX] = index;
		},
			[]() {});
	}

	size_t GetCount(const std::string& layoutName, const std::string& menuId)
	{
		return WithMenu<size_t>(layoutName, menuId, [](auto& thingie)
		{
			return thingie[graphics::Properties::MENU_ITEMS].size();
		},
			[]() { return 0; });
	}

	static void ChangeMenuIndex(const std::string& layoutName, const std::string& menuId, int delta)
	{
		auto index = Read(layoutName, menuId);
		if (index)
		{
			auto itemCount = GetCount(layoutName, menuId);
			if (itemCount > 0)
			{
				Write(layoutName, menuId, (*index + (int)itemCount + delta) % itemCount);
			}
			else
			{
				Write(layoutName, menuId, 0);
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