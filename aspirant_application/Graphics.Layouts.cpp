#include "Graphics.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Graphics.Data.Properties.h"
#include "Graphics.Layout.h"
#include "Data.Stores.h"
namespace graphics::Menu { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace graphics::Image { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace graphics::Text { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace graphics::WorldMap { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace graphics::Sublayout { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace graphics::FloorInventory { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace graphics::AvatarInventory { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace graphics::Layout
{
	static std::map<std::string, void(*)(std::shared_ptr<SDL_Renderer>, const nlohmann::json&)> table =
	{
		{graphics::data::Types::IMAGE, graphics::Image::Draw},
		{graphics::data::Types::MENU, graphics::Menu::Draw},
		{graphics::data::Types::TEXT, graphics::Text::Draw},
		{graphics::data::Types::WORLD_MAP, graphics::WorldMap::Draw},
		{graphics::data::Types::LAYOUT, graphics::Sublayout::Draw},
		{graphics::data::Types::FLOOR_INVENTORY, graphics::FloorInventory::Draw},
		{graphics::data::Types::AVATAR_INVENTORY, graphics::AvatarInventory::Draw}
	};

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		for (auto& drawn : model)
		{
			std::string drawnType = drawn[common::data::Properties::TYPE];
			auto drawer = table.find(drawnType);
			if (drawer != table.end())
			{
				drawer->second(renderer, drawn);
			}
		}
	}
}
namespace graphics::Layouts
{
	std::map<std::string, nlohmann::json> layouts;

	void InitializeLayout(const std::string& layoutName)
	{
		if (!layouts.contains(layoutName))
		{
			layouts[layoutName] = ::data::JSON::Load(::data::Stores::GetStore(::data::Store::LAYOUTS)[layoutName]);
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
		graphics::Layout::Draw(renderer, layouts[layoutName]);
	}
}