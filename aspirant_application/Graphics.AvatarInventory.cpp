#include "Graphics.AvatarInventory.h"
#include <SDL.h>
#include "json.hpp"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
#include "Game.Avatar.Items.h"
#include <sstream>
#include "Graphics.Fonts.h"
#include "Game.Items.h"
namespace graphics::AvatarInventory
{
	static size_t inventoryIndex;

	void ResetIndex()
	{
		inventoryIndex = 0;
	}

	void NextIndex()
	{
		auto inventory = game::avatar::Items::All();
		if (!inventory.empty())
		{
			inventoryIndex = (inventoryIndex + 1) % inventory.size();
		}
	}

	void PreviousIndex()
	{
		auto inventory = game::avatar::Items::All();
		if (!inventory.empty())
		{
			inventoryIndex = (inventoryIndex + inventory.size() - 1) % inventory.size();
		}
	}

	std::optional<game::Item> GetItem()
	{
		auto& inventory = game::avatar::Items::All();
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
		int x = model[common::Properties::X];
		int y = model[common::Properties::Y];
		int rowHeight = model[graphics::Properties::ROW_HEIGHT];
		std::string font = model[graphics::Properties::FONT];
		std::string inactiveColor = model[graphics::Properties::COLORS][graphics::Properties::INACTIVE];
		std::string activeColor = model[graphics::Properties::COLORS][graphics::Properties::ACTIVE];
		bool dropShadow = model[graphics::Properties::DROP_SHADOW];
		int dropShadowX = model[graphics::Properties::DROP_SHADOW_X];
		int dropShadowY = model[graphics::Properties::DROP_SHADOW_Y];
		std::string dropShadowColor = model[graphics::Properties::DROP_SHADOW_COLOR];


		auto& inventory = game::avatar::Items::All();
		if (inventoryIndex >= inventory.size())
		{
			inventoryIndex = 0;
		}

		size_t index = 0;
		for (auto& entry : inventory)
		{
			std::stringstream ss;
			ss << game::Items::GetName(entry.first) << " x " << entry.second;
			auto color = (index == inventoryIndex) ? (activeColor) : (inactiveColor);
			if (dropShadow)
			{
				graphics::Fonts::Read(font).value().WriteText(renderer, { x + dropShadowX,y + dropShadowY }, ss.str(), dropShadowColor, graphics::HorizontalAlignment::LEFT);
			}
			graphics::Fonts::Read(font).value().WriteText(renderer, { x,y }, ss.str(), color, graphics::HorizontalAlignment::LEFT);
			y += rowHeight;
			index++;
		}

		if (index == 0)
		{
			if (dropShadow)
			{
				graphics::Fonts::Read(font).value().WriteText(renderer, { x + dropShadowX,y + dropShadowY }, "(nothing)", dropShadowColor, graphics::HorizontalAlignment::LEFT);
			}
			graphics::Fonts::Read(font).value().WriteText(renderer, { x,y }, "(nothing)", inactiveColor, graphics::HorizontalAlignment::LEFT);
		}
	}
}