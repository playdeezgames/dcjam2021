#include "Visuals.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Data.Stores.h"
#include <functional>
namespace visuals::Menu 
{ 
	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::Image 
{ 
	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::Text 
{ 
	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::WorldMap 
{ 
	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::Sublayout 
{ 
	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::FloorInventory 
{ 
	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::AvatarInventory 
{ 
	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::Layout
{
	struct InternalLayout
	{
		std::vector<std::function<void(std::shared_ptr<SDL_Renderer>)>> drawer;
	};
}
namespace visuals::Layouts
{
	struct InternalLayout
	{
		std::vector<std::function<void(std::shared_ptr<SDL_Renderer>)>> drawers;
	};

	static std::map<std::string, nlohmann::json> layouts;
	static std::map<std::string, InternalLayout> internalLayouts;

	static void Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		internalLayouts[layoutName] = {};
		for (auto& drawn : model)
		{
			auto drawnType = visuals::data::Types::FromString(drawn[common::data::Properties::TYPE]);
			if (drawnType)
			{
				switch (drawnType.value())
				{
				case visuals::data::Type::IMAGE:
					internalLayouts[layoutName].drawers.push_back(visuals::Image::Internalize(layoutName, drawn));
					break;
				case visuals::data::Type::TEXT:
					internalLayouts[layoutName].drawers.push_back(visuals::Text::Internalize(layoutName, drawn));
					break;
				case visuals::data::Type::WORLD_MAP:
					internalLayouts[layoutName].drawers.push_back(visuals::WorldMap::Internalize(layoutName, drawn));
					break;
				case visuals::data::Type::MENU:
					internalLayouts[layoutName].drawers.push_back(visuals::Menu::Internalize(layoutName, drawn));
					break;
				case visuals::data::Type::LAYOUT:
					internalLayouts[layoutName].drawers.push_back(visuals::Sublayout::Internalize(layoutName, drawn));
					break;
				case visuals::data::Type::FLOOR_INVENTORY:
					internalLayouts[layoutName].drawers.push_back(visuals::FloorInventory::Internalize(layoutName, drawn));
					break;
				case visuals::data::Type::AVATAR_INVENTORY:
					internalLayouts[layoutName].drawers.push_back(visuals::AvatarInventory::Internalize(layoutName, drawn));
					break;
				}
			}
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

	nlohmann::json& GetLayout(const std::string& layoutName)
	{
		InitializeLayout(layoutName);
		return layouts.find(layoutName)->second;
	}

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const std::string& layoutName)
	{
		InitializeLayout(layoutName);
		auto& internalLayout = internalLayouts[layoutName];
		for (auto& drawer : internalLayout.drawers)
		{
			drawer(renderer);
		}
	}
}