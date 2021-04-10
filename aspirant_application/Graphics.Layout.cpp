#include "Common.Utility.h"
#include "Common.Properties.h"
#include "Graphics.Types.h"
#include <map>
#include "json.hpp"
#include <SDL.h>
#include "Graphics.Layouts.h"
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
		{graphics::Types::IMAGE, graphics::Image::Draw},
		{graphics::Types::MENU, graphics::Menu::Draw},
		{graphics::Types::TEXT, graphics::Text::Draw},
		{graphics::Types::WORLD_MAP, graphics::WorldMap::Draw},
		{graphics::Types::LAYOUT, graphics::Sublayout::Draw},
		{graphics::Types::FLOOR_INVENTORY, graphics::FloorInventory::Draw},
		{graphics::Types::AVATAR_INVENTORY, graphics::AvatarInventory::Draw}
	};

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		for (auto& drawn : model)
		{
			std::string drawnType = drawn[common::Properties::TYPE];
			auto drawer = table.find(drawnType);
			if (drawer != table.end())
			{
				drawer->second(renderer, drawn);
			}
		}
	}
}