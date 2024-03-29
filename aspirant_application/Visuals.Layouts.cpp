#include "Visuals.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Data.Stores.h"
#include <functional>
namespace visuals
{
	typedef std::function<void(const std::shared_ptr<SDL_Renderer>&)> DrawerFunction;
}
namespace visuals::Menu 
{ 
	DrawerFunction Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::Image 
{ 
	DrawerFunction Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::Text 
{ 
	DrawerFunction Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::WorldMap 
{ 
	DrawerFunction Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::Sublayout 
{ 
	DrawerFunction Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::AvatarInventory 
{ 
	DrawerFunction Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::Layouts
{
	struct InternalLayout
	{
		std::vector<DrawerFunction> drawers;
	};

	static std::map<std::string, nlohmann::json> layouts;
	static std::map<std::string, InternalLayout> internalLayouts;

	typedef std::function<DrawerFunction(const std::string&, const nlohmann::json&)> InternalizerFunction;

	static std::map<visuals::data::Type, InternalizerFunction> internalizers =
	{
		{visuals::data::Type::IMAGE, visuals::Image::Internalize},
		{visuals::data::Type::TEXT, visuals::Text::Internalize},
		{visuals::data::Type::WORLD_MAP, visuals::WorldMap::Internalize},
		{visuals::data::Type::MENU, visuals::Menu::Internalize},
		{visuals::data::Type::LAYOUT, visuals::Sublayout::Internalize},
		{visuals::data::Type::AVATAR_INVENTORY, visuals::AvatarInventory::Internalize},
		{visuals::data::Type::AREA, [](const std::string&, const nlohmann::json&) { return [](const std::shared_ptr<SDL_Renderer>&) {};  }}
	};

	static void InternalizeTypedDrawn(const std::string layoutName, visuals::data::Type drawnType, const nlohmann::json& drawn)
	{
		internalLayouts[layoutName].drawers.push_back(internalizers.find(drawnType)->second(layoutName, drawn));
	}

	static void InternalizeTypedDrawnFromString(const std::string layoutName, const nlohmann::json& type, const nlohmann::json& drawn)
	{
		auto drawnType = visuals::data::Types::FromString(type);
		if (drawnType)
		{
			InternalizeTypedDrawn(layoutName, *drawnType, drawn);
		}
	}

	static void InternalizeDrawn(const std::string layoutName, const nlohmann::json& drawn)
	{
		auto& types = drawn[common::data::Properties::TYPE];
		if (types.is_array())
		{
			for (auto& type : types)
			{
				InternalizeTypedDrawnFromString(layoutName, type, drawn);
			}
			return;
		}
		InternalizeTypedDrawnFromString(layoutName, types, drawn);
	}

	static void Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		internalLayouts[layoutName] = {};
		for (auto& drawn : model)
		{
			InternalizeDrawn(layoutName, drawn);
		}
	}

	static void InitializeLayout(const std::string& layoutName)
	{
		if (!layouts.contains(layoutName))
		{
			layouts[layoutName] = ::data::JSON::Load(::data::Stores::GetStore(::data::Store::LAYOUTS)[layoutName]);
			Internalize(layoutName, layouts[layoutName]);
		}
	}

	void Start()
	{
		auto& store = ::data::Stores::GetStore(::data::Store::LAYOUTS);
		for (auto& entry : store.items())
		{
			InitializeLayout(entry.key());
		}
	}

	nlohmann::json& GetLayout(const std::string& layoutName)
	{
		InitializeLayout(layoutName);
		return layouts.find(layoutName)->second;
	}

	void Draw(const std::shared_ptr<SDL_Renderer>& renderer, const std::string& layoutName)
	{
		InitializeLayout(layoutName);
		auto& internalLayout = internalLayouts[layoutName];
		for (auto& drawer : internalLayout.drawers)
		{
			drawer(renderer);
		}
	}
}