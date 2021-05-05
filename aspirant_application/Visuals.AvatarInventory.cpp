#include "Visuals.AvatarInventory.h"
#include "json.hpp"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Game.Avatar.Items.h"
#include <sstream>
#include "Visuals.Fonts.h"
#include "Game.Item.h"
#include "Visuals.Data.Types.h"
#include <optional>
namespace visuals::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace visuals::AvatarInventory
{
	static size_t inventoryIndex;//TODO: this is broken, and "should" be per control, but as there is ever only one on a screen and only one screen it is on.... JAWTS

	struct InternalAvatarInventory
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

	static std::vector<InternalAvatarInventory> internalAvatarInventories;

	static void DrawInternalAvatarInventory(std::shared_ptr<SDL_Renderer> renderer, size_t avatarInventoryIndex)
	{
		auto& avatarInventory = internalAvatarInventories[avatarInventoryIndex];

		common::XY<int> xy = avatarInventory.xy;
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
			auto color = (index == inventoryIndex) ? (avatarInventory.activeColor) : (avatarInventory.inactiveColor);
			if (avatarInventory.dropShadow)
			{
				visuals::Fonts::WriteText(avatarInventory.font, renderer, xy + avatarInventory.dropShadowXY, ss.str(), avatarInventory.dropShadowColor, visuals::HorizontalAlignment::LEFT);
			}
			visuals::Fonts::WriteText(avatarInventory.font, renderer, xy, ss.str(), color, visuals::HorizontalAlignment::LEFT);
			xy = xy + common::XY<int>(0, avatarInventory.rowHeight);
			index++;
		}

		if (index == 0)
		{
			if (avatarInventory.dropShadow)
			{
				visuals::Fonts::WriteText(avatarInventory.font, renderer, avatarInventory.xy + avatarInventory.dropShadowXY, "(nothing)", avatarInventory.dropShadowColor, visuals::HorizontalAlignment::LEFT);
			}
			visuals::Fonts::WriteText(avatarInventory.font, renderer, avatarInventory.xy, "(nothing)", avatarInventory.inactiveColor, visuals::HorizontalAlignment::LEFT);
		}

	}

	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		size_t index = internalAvatarInventories.size();
		internalAvatarInventories.push_back(
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
			DrawInternalAvatarInventory(renderer, index);
		};
	}

	template <typename TResult>
	static TResult WithControl(const std::string& layoutName, const std::string& controlId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : visuals::Layouts::GetLayout(layoutName))
		{
			if (visuals::data::Types::FromString(thingie[common::data::Properties::TYPE]) == visuals::data::Type::AVATAR_INVENTORY)
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
		if (inventory.size() == 0)
		{
			return std::nullopt;
		}
		else if (inventoryIndex < inventory.size())
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
			inventoryIndex = 0;
			return GetItem();
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
			int rowHeight = thingie[visuals::data::Properties::ROW_HEIGHT];
			if (xy.GetX() >= x && xy.GetX() < x + width && xy.GetY() >= y)
			{
				size_t row = ((size_t)xy.GetY() - (size_t)y) / (size_t)rowHeight;
				auto inventory = game::avatar::Items::All();
				if ((size_t)row < inventory.size())
				{
					return std::optional<int>((int)row);
				}
			}
			return std::optional<int>();
		}, []() { return std::optional<int>(); });
	}
}