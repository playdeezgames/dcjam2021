#include "Visuals.Menus.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Layouts.h"
namespace graphics::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace graphics::Menus
{
	template<typename TResult>
	static TResult WithMenu(const std::string& layoutName, const std::string& menuId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : graphics::Layouts::GetLayout(layoutName))
		{
			if (graphics::data::Types::FromString(thingie[common::data::Properties::TYPE]) == graphics::data::Type::MENU &&
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

	static std::optional<int> FindIndexForValue(const std::string& layoutName, const std::string& menuId, int value)
	{
		return WithMenu<std::optional<int>>(layoutName, menuId, [value](auto& thingie) 
		{ 
			int index = 0;
			for (auto& menuItem : thingie[graphics::data::Properties::MENU_ITEMS])
			{
				if (value == (int)menuItem[graphics::data::Properties::VALUE])
				{
					return std::optional<int>(index);
				}
				index++;
			}
			return std::optional<int>();
		}, 
			[]() { return std::nullopt;  });
	}

	bool WriteValue(const std::string& layoutName, const std::string& menuId, int value)
	{
		auto index = graphics::Menus::FindIndexForValue(layoutName, menuId, value);
		if (index)
		{
			graphics::Menus::WriteIndex(layoutName, menuId, *index);
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