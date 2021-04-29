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

	static size_t inventoryIndex = 0;

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