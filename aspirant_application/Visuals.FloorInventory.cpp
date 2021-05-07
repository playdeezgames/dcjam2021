#include "json.hpp"
#include <SDL.h>
#include "Game.Item.h"
#include "Game.World.Items.h"
#include "Common.Data.Properties.h"
#include "Game.Item.h"
#include "Game.Avatar.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Fonts.h"
#include "Visuals.Data.Types.h"
#include <sstream>
namespace visuals::FloorInventory
{
	struct InternalFloorInventory
	{
		common::XY<int> xy;
		int width;
		int rowHeight;
		std::string font;
		std::string inactiveColor;
		std::string activeColor;
		bool dropShadow;
		common::XY<int> dropShadowXY;
		std::string dropShadowColor;
	};

	static std::vector<InternalFloorInventory> internalFloorInventories;
	static std::map<std::string, std::map<std::string, size_t>> floorInventoryTable;
	static size_t inventoryIndex = 0;

	static std::map<int, size_t> GetInventory()
	{
		auto location = game::Avatar::GetPosition();
		return game::world::Items::FloorInventory(location);
	}

	static void DrawInternalFloorInventory(std::shared_ptr<SDL_Renderer> renderer, size_t floorInventoryIndex)
	{
		auto& floorInventory = internalFloorInventories[floorInventoryIndex];
		auto xy = floorInventory.xy;

		auto inventory = GetInventory();

		if (inventoryIndex >= inventory.size())
		{
			inventoryIndex = 0;
		}

		size_t index = 0;
		for (auto& entry : inventory)
		{
			std::stringstream ss;
			ss << game::item::GetDescriptor(entry.first).name << " x " << entry.second;
			auto color = (index == inventoryIndex) ? (floorInventory.activeColor) : (floorInventory.inactiveColor);
			if (floorInventory.dropShadow)
			{
				visuals::Fonts::WriteText(floorInventory.font, renderer, xy + floorInventory.dropShadowXY, ss.str(), floorInventory.dropShadowColor, visuals::HorizontalAlignment::LEFT);
			}
			visuals::Fonts::WriteText(floorInventory.font, renderer, xy, ss.str(), color, visuals::HorizontalAlignment::LEFT);
			xy = xy + common::XY<int>(0, floorInventory.rowHeight);
			index++;
		}

		if (index == 0)
		{
			if (floorInventory.dropShadow)
			{
				visuals::Fonts::WriteText(floorInventory.font, renderer, floorInventory.xy + floorInventory.dropShadowXY, "(nothing)", floorInventory.dropShadowColor, visuals::HorizontalAlignment::LEFT);
			}
			visuals::Fonts::WriteText(floorInventory.font, renderer, floorInventory.xy, "(nothing)", floorInventory.inactiveColor, visuals::HorizontalAlignment::LEFT);
		}
	}

	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		size_t index = internalFloorInventories.size();
		internalFloorInventories.push_back(
			{
				common::XY<int>(model[common::data::Properties::X], model[common::data::Properties::Y]),
				model[common::data::Properties::WIDTH],
				model[visuals::data::Properties::ROW_HEIGHT],
				model[visuals::data::Properties::FONT],
				model[visuals::data::Properties::COLORS][visuals::data::Properties::INACTIVE],
				model[visuals::data::Properties::COLORS][visuals::data::Properties::ACTIVE],
				model[visuals::data::Properties::DROP_SHADOW],
				common::XY<int>(model[visuals::data::Properties::DROP_SHADOW_X],model[visuals::data::Properties::DROP_SHADOW_Y]),
				model[visuals::data::Properties::DROP_SHADOW_COLOR]
			});
		if (model.count(visuals::data::Properties::CONTROL_ID) > 0)
		{
			floorInventoryTable[layoutName][model[visuals::data::Properties::CONTROL_ID]] = index;
		}
		return [index](std::shared_ptr<SDL_Renderer> renderer) 
		{
			DrawInternalFloorInventory(renderer, index);
		};
	}

	void ResetIndex()
	{
		inventoryIndex = 0;
	}

	void NextIndex()
	{
		//
		auto inventory = GetInventory();

		if (!inventory.empty())
		{
			inventoryIndex = (inventoryIndex + 1) % inventory.size();
		}
	}

	void PreviousIndex()
	{
		auto inventory = GetInventory();

		if (!inventory.empty())
		{
			inventoryIndex = (inventoryIndex + inventory.size() - 1) % inventory.size();
		}
	}

	std::optional<int> GetItem()
	{
		auto inventory = GetInventory();

		if (inventoryIndex < inventory.size())
		{
			auto iter = inventory.begin();
			for (size_t dummy = 0; dummy < inventoryIndex; ++dummy)
			{
				iter++;
			}
			return iter->first;
		}
		else
		{
			return std::nullopt;
		}
	}

	void OnMouseMotion(const std::string& layoutName, const std::string& controlId, const common::XY<Sint32>& xy)
	{
		auto floorInventoryIndex = floorInventoryTable[layoutName][controlId];
		auto& floorInventory = internalFloorInventories[floorInventoryIndex];

		int x = floorInventory.xy.GetX();
		int y = floorInventory.xy.GetY();
		int width = floorInventory.width;
		int rowHeight = floorInventory.rowHeight;
		if (xy.GetX() >= x && xy.GetX() < x + width && xy.GetY() >= y)
		{
			size_t row = ((size_t)xy.GetY() - (size_t)y) / (size_t)rowHeight;

			auto inventory = GetInventory();

			if ((size_t)row < inventory.size())
			{
				inventoryIndex = row;
			}
		}
	}

	std::optional<int> OnMouseButtonUp(const std::string& layoutName, const std::string& controlId, const common::XY<Sint32>& xy, Uint8 buttons)
	{
		auto floorInventoryIndex = floorInventoryTable[layoutName][controlId];
		auto& floorInventory = internalFloorInventories[floorInventoryIndex];
		int x = floorInventory.xy.GetX();
		int y = floorInventory.xy.GetY();
		int width = floorInventory.width;
		int rowHeight = floorInventory.rowHeight;
		if (xy.GetX() >= x && xy.GetX() < x + width && xy.GetY() >= y)
		{
			size_t row = ((size_t)xy.GetY() - (size_t)y) / (size_t)rowHeight;
			auto inventory = GetInventory();
			if ((size_t)row < inventory.size())
			{
				return std::optional<int>((int)row);
			}
		}
		return std::optional<int>();
	}
}