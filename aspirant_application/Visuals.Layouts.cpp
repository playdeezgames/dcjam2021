#include "Visuals.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Data.Stores.h"
namespace visuals::Menu { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace visuals::Image { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace visuals::Text { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace visuals::WorldMap { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace visuals::Sublayout { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace visuals::FloorInventory { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace visuals::AvatarInventory { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace visuals::Layout
{
	static std::map<visuals::data::Type, std::function<void(std::shared_ptr<SDL_Renderer>, const nlohmann::json&)>> table =
	{
		{visuals::data::Type::IMAGE, visuals::Image::Draw},
		{visuals::data::Type::MENU, visuals::Menu::Draw},
		{visuals::data::Type::TEXT, visuals::Text::Draw},
		{visuals::data::Type::WORLD_MAP, visuals::WorldMap::Draw},
		{visuals::data::Type::LAYOUT, visuals::Sublayout::Draw},
		{visuals::data::Type::FLOOR_INVENTORY, visuals::FloorInventory::Draw},
		{visuals::data::Type::AVATAR_INVENTORY, visuals::AvatarInventory::Draw}
	};

	std::map<visuals::data::Type, size_t> typeRenderCounts;
	std::map<visuals::data::Type, Uint32> typeRenderTimes;
	std::map<visuals::data::Type, Uint32> averageTypeRenderTimes;

	static void LogTypeRender(visuals::data::Type visualType, Uint32 renderTime)
	{
		typeRenderCounts[visualType]++;
		typeRenderTimes[visualType] += renderTime;
		averageTypeRenderTimes[visualType] = typeRenderTimes[visualType] / typeRenderCounts[visualType];
	}

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		for (auto& drawn : model)
		{
			auto drawnType = visuals::data::Types::FromString(drawn[common::data::Properties::TYPE]);
			if (drawnType)
			{
				auto drawer = table.find(*drawnType);
				if (drawer != table.end())
				{
					auto start = SDL_GetTicks();
					drawer->second(renderer, drawn);
					LogTypeRender(drawnType.value(), SDL_GetTicks() - start);
				}
			}
		}
	}
}
namespace visuals::Layouts
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

	static std::map<std::string, size_t> renderCounts;
	static std::map<std::string, Uint32> renderTimes;
	static std::map<std::string, Uint32> averageRenderTimes;

	static void LogRender(const std::string& layoutName, Uint32 renderTime)
	{
		renderCounts[layoutName]++;
		renderTimes[layoutName] += renderTime;
		averageRenderTimes[layoutName] = renderTimes[layoutName] / renderCounts[layoutName];
	}

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const std::string& layoutName)
	{
		InitializeLayout(layoutName);
		Uint32 start = SDL_GetTicks();
		visuals::Layout::Draw(renderer, layouts[layoutName]);
		LogRender(layoutName, SDL_GetTicks() - start);
	}
}