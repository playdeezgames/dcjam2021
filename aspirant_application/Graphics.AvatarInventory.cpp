#include "Graphics.AvatarInventory.h"
#include <SDL.h>
#include "json.hpp"
#include "Common.Data.Properties.h"
#include "Graphics.Data.Properties.h"
#include "Game.Avatar.Items.h"
#include <sstream>
#include "Graphics.Fonts.h"
#include "Game.Item.h"
#include "Graphics.Data.Types.h"
namespace graphics::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace graphics::AvatarInventory
{
	template <typename TResult>
	static TResult WithControl(const std::string& layoutName, const std::string& controlId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : graphics::Layouts::GetLayout(layoutName))
		{
			if (graphics::data::Types::FromString(thingie[common::data::Properties::TYPE]) == graphics::data::Type::AVATAR_INVENTORY)
			{
				if (thingie.count(graphics::data::Properties::CONTROL_ID) > 0 &&
					thingie[graphics::data::Properties::CONTROL_ID] == controlId)
				{
					return func(thingie);
				}
			}
		}
		return notFound();
	}

	static size_t inventoryIndex;//TODO: this is broken, and "should" be per control, but as there is ever only one on a screen and only one screen it is on.... JAWTS

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

	std::optional<int> GetItem()
	{
		auto inventory = game::avatar::Items::All();
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
		int rowHeight = model[graphics::data::Properties::ROW_HEIGHT];
		std::string font = model[graphics::data::Properties::FONT];
		std::string inactiveColor = model[graphics::data::Properties::COLORS][graphics::data::Properties::INACTIVE];
		std::string activeColor = model[graphics::data::Properties::COLORS][graphics::data::Properties::ACTIVE];
		bool dropShadow = model[graphics::data::Properties::DROP_SHADOW];
		int dropShadowX = model[graphics::data::Properties::DROP_SHADOW_X];
		int dropShadowY = model[graphics::data::Properties::DROP_SHADOW_Y];
		std::string dropShadowColor = model[graphics::data::Properties::DROP_SHADOW_COLOR];


		auto inventory = game::avatar::Items::All();
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
				graphics::Fonts::WriteText(font, renderer, { x + dropShadowX,y + dropShadowY }, ss.str(), dropShadowColor, graphics::HorizontalAlignment::LEFT);
			}
			graphics::Fonts::WriteText(font, renderer, { x,y }, ss.str(), color, graphics::HorizontalAlignment::LEFT);
			y += rowHeight;
			index++;
		}

		if (index == 0)
		{
			if (dropShadow)
			{
				graphics::Fonts::WriteText(font, renderer, { x + dropShadowX,y + dropShadowY }, "(nothing)", dropShadowColor, graphics::HorizontalAlignment::LEFT);
			}
			graphics::Fonts::WriteText(font, renderer, { x,y }, "(nothing)", inactiveColor, graphics::HorizontalAlignment::LEFT);
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
			int rowHeight = thingie[graphics::data::Properties::ROW_HEIGHT];
			if (xy.GetX() >= x && xy.GetX()<x+width && xy.GetY()>=y)
			{
				size_t row = ((size_t)xy.GetY() - (size_t)y) / (size_t)rowHeight;
				auto inventory = game::avatar::Items::All();
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
			int rowHeight = thingie[graphics::data::Properties::ROW_HEIGHT];
			if (xy.GetX() >= x && xy.GetX() < x + width && xy.GetY() >= y)
			{
				size_t row = ((size_t)xy.GetY() - (size_t)y) / (size_t)rowHeight;
				auto inventory = game::avatar::Items::All();
				if ((size_t)row < inventory.size())
				{
					return std::optional<int>(row);
				}
			}
			return std::optional<int>();
		}, []() { return std::nullopt; });
	}
}