#include "Graphics.Menus.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Graphics.Data.Properties.h"
#include "Graphics.Layouts.h"
namespace graphics::Menus
{
	template<typename TResult>
	static TResult WithMenu(const std::string& layoutName, const std::string& menuId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : graphics::Layouts::GetLayout(layoutName))
		{
			if (thingie[common::data::Properties::TYPE] == graphics::data::Types::MENU &&
				thingie[graphics::data::Properties::MENU_ID] == menuId)
			{
				return func(thingie);
			}
		}
		return notFound();
	}

	std::optional<int> ReadIndex(const std::string& layoutName, const std::string& menuId)
	{
		return WithMenu<std::optional<int>>(layoutName, menuId, [](nlohmann::json& thingie)
		{
			return thingie[graphics::data::Properties::INDEX];
		},
			[]() {return std::nullopt; });
	}

	std::optional<int> ReadValue(const std::string& layoutName, const std::string& menuId)
	{
		return WithMenu<std::optional<int>>(layoutName, menuId, [](nlohmann::json& thingie)
		{
			int index = thingie[graphics::data::Properties::INDEX];
			return (int)thingie[graphics::data::Properties::MENU_ITEMS][index][graphics::data::Properties::VALUE];
		},
			[]() {return std::nullopt; });
	}

	void WriteIndex(const std::string& layoutName, const std::string& menuId, int index)
	{
		return WithMenu<void>(layoutName, menuId, [index](nlohmann::json& thingie)
		{
			thingie[graphics::data::Properties::INDEX] = index;
		},
			[]() {});
	}

	size_t GetCount(const std::string& layoutName, const std::string& menuId)
	{
		return WithMenu<size_t>(layoutName, menuId, [](auto& thingie)
		{
			return thingie[graphics::data::Properties::MENU_ITEMS].size();
		},
			[]() { return 0; });
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

	void Next(const std::string& layoutName, const std::string& menuId)
	{
		ChangeMenuIndex(layoutName, menuId, 1);
	}

	void Previous(const std::string& layoutName, const std::string& menuId)
	{
		ChangeMenuIndex(layoutName, menuId, -1);
	}

}