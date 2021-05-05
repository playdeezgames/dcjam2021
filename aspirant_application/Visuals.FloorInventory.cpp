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
namespace visuals::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace visuals::FloorInventory
{
	struct InternalFloorInventory
	{
		common::XY<int> xy;
		int rowHeight;
		std::string font;
		std::string inactiveColor;
		std::string activeColor;
		bool dropShadow;
		common::XY<int> dropShadowXY;
		std::string dropShadowColor;
	};

	static std::vector<InternalFloorInventory> internalFloorInventories;
	static size_t inventoryIndex = 0;

	static void DrawInternalFloorInventory(std::shared_ptr<SDL_Renderer> renderer, size_t floorInventoryIndex)
	{
		auto& floorInventory = internalFloorInventories[floorInventoryIndex];
		auto xy = floorInventory.xy;

		auto location = game::Avatar::GetPosition();
		auto inventory = game::world::Items::FloorInventory(location);
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
				model[visuals::data::Properties::ROW_HEIGHT],
				model[visuals::data::Properties::FONT],
				model[visuals::data::Properties::COLORS][visuals::data::Properties::INACTIVE],
				model[visuals::data::Properties::COLORS][visuals::data::Properties::ACTIVE],
				model[visuals::data::Properties::DROP_SHADOW],
				common::XY<int>(model[visuals::data::Properties::DROP_SHADOW_X],model[visuals::data::Properties::DROP_SHADOW_Y]),
				model[visuals::data::Properties::DROP_SHADOW_COLOR]
			});
		return [index](std::shared_ptr<SDL_Renderer> renderer) 
		{
			DrawInternalFloorInventory(renderer, index);
		};
	}

	template <typename TResult>
	static TResult WithControl(const std::string& layoutName, const std::string& controlId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : visuals::Layouts::GetLayout(layoutName))
		{
			if (visuals::data::Types::FromString(thingie[common::data::Properties::TYPE]) == visuals::data::Type::FLOOR_INVENTORY)
			{
				if (thingie.count(visuals::data::Properties::CONTROL_ID) > 0 &&
					thingie[visuals::data::Properties::CONTROL_ID] == controlId)
				{
					return func(thingie);
				}
			}
		}
		return notFound();
	}

	void ResetIndex()
	{
		inventoryIndex = 0;
	}

	void NextIndex()
	{
		auto location = game::Avatar::GetPosition();
		auto inventory = game::world::Items::FloorInventory(location);
		if (!inventory.empty())
		{
			inventoryIndex = (inventoryIndex + 1) % inventory.size();
		}
	}

	void PreviousIndex()
	{
		auto location = game::Avatar::GetPosition();
		auto inventory = game::world::Items::FloorInventory(location);
		if (!inventory.empty())
		{
			inventoryIndex = (inventoryIndex + inventory.size() - 1) % inventory.size();
		}
	}

	std::optional<int> GetItem()
	{
		auto location = game::Avatar::GetPosition();
		auto inventory = game::world::Items::FloorInventory(location);
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

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		int x = model[common::data::Properties::X];
		int y = model[common::data::Properties::Y];
		int rowHeight = model[visuals::data::Properties::ROW_HEIGHT];
		std::string font = model[visuals::data::Properties::FONT];
		std::string inactiveColor = model[visuals::data::Properties::COLORS][visuals::data::Properties::INACTIVE];
		std::string activeColor = model[visuals::data::Properties::COLORS][visuals::data::Properties::ACTIVE];
		bool dropShadow = model[visuals::data::Properties::DROP_SHADOW];
		int dropShadowX = model[visuals::data::Properties::DROP_SHADOW_X];
		int dropShadowY = model[visuals::data::Properties::DROP_SHADOW_Y];
		std::string dropShadowColor = model[visuals::data::Properties::DROP_SHADOW_COLOR];


		auto location = game::Avatar::GetPosition();
		auto inventory = game::world::Items::FloorInventory(location);
		if (inventoryIndex >= inventory.size())
		{
			inventoryIndex = 0;
		}

		size_t index = 0;
		for (auto& entry : inventory)
		{
			std::stringstream ss;
			ss << game::item::GetDescriptor(entry.first).name << " x " << entry.second;
			auto color = (index == inventoryIndex) ? (activeColor) : (inactiveColor);
			if (dropShadow)
			{
				visuals::Fonts::WriteText(font, renderer, { x + dropShadowX,y + dropShadowY }, ss.str(), dropShadowColor, visuals::HorizontalAlignment::LEFT);
			}
			visuals::Fonts::WriteText(font, renderer, { x,y }, ss.str(), color, visuals::HorizontalAlignment::LEFT);
			y += rowHeight;
			index++;
		}

		if (index == 0)
		{
			if (dropShadow)
			{
				visuals::Fonts::WriteText(font, renderer, { x + dropShadowX,y + dropShadowY }, "(nothing)", dropShadowColor, visuals::HorizontalAlignment::LEFT);
			}
			visuals::Fonts::WriteText(font, renderer, { x,y }, "(nothing)", inactiveColor, visuals::HorizontalAlignment::LEFT);
		}
	}

	void OnMouseMotion(const std::string& layoutName, const std::string& controlId, const common::XY<Sint32>& xy)
	{
		WithControl<void>(layoutName, controlId,
			[xy](nlohmann::json& thingie)
		{
			int x = thingie[common::data::Properties::X];
			int y = thingie[common::data::Properties::Y];
			int width = thingie[common::data::Properties::WIDTH];
			int rowHeight = thingie[visuals::data::Properties::ROW_HEIGHT];
			
			if (xy.GetX() >= x && xy.GetX() < x + width && xy.GetY() >= y)
			{
				size_t row = ((size_t)xy.GetY() - (size_t)y) / (size_t)rowHeight;
				auto inventory = game::world::Items::FloorInventory(game::Avatar::GetPosition());
				if ((size_t)row < inventory.size())
				{
					inventoryIndex = row;
				}
			}
		}, []() {});
	}

	std::optional<int> OnMouseButtonUp(const std::string& layoutName, const std::string& controlId, const common::XY<Sint32>& xy, Uint8 buttons)
	{
		return WithControl<std::optional<int>>(layoutName, controlId,
			[xy, buttons](nlohmann::json& thingie)
		{
			int x = thingie[common::data::Properties::X];
			int y = thingie[common::data::Properties::Y];
			int width = thingie[common::data::Properties::WIDTH];
			int rowHeight = thingie[visuals::data::Properties::ROW_HEIGHT];
			if (xy.GetX() >= x && xy.GetX() < x + width && xy.GetY() >= y)
			{
				size_t row = ((size_t)xy.GetY() - (size_t)y) / (size_t)rowHeight;
				auto inventory = game::world::Items::FloorInventory(game::Avatar::GetPosition());
				if (row < inventory.size())
				{
					return std::optional<int>((int)row);
				}
			}
			return std::optional<int>();
		}, []() { return std::optional<int>(); });
	}
}