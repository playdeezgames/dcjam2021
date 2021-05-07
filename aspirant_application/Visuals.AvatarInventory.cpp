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
#include "Game.World.Items.h"
#include "Game.Avatar.h"
namespace visuals::AvatarInventory
{
	enum class InventorySource
	{
		AVATAR,
		FLOOR
	};

	struct InternalAvatarInventory
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
		size_t inventoryIndex;
		InventorySource source;
	};

	static std::vector<InternalAvatarInventory> internalAvatarInventories;
	static std::map<std::string, std::map<std::string, size_t>> avatarInventoryTable;

	static size_t GetInventoryIndex(const std::string& layoutName, const std::string& controlId)
	{
		return internalAvatarInventories[avatarInventoryTable.find(layoutName)->second.find(controlId)->second].inventoryIndex;
	}

	static void SetInventoryIndex(const std::string& layoutName, const std::string& controlId, size_t value)
	{
		internalAvatarInventories[avatarInventoryTable.find(layoutName)->second.find(controlId)->second].inventoryIndex = value;
	}

	static std::map<int, size_t> GetInventory(InventorySource source)
	{
		if (source == InventorySource::AVATAR)
		{
			return game::avatar::Items::All();
		}
		return game::world::Items::FloorInventory(game::Avatar::GetPosition());
	}

	static void DrawInternalAvatarInventory(std::shared_ptr<SDL_Renderer> renderer, size_t avatarInventoryIndex)
	{
		auto& avatarInventory = internalAvatarInventories[avatarInventoryIndex];

		common::XY<int> xy = avatarInventory.xy;
		auto inventory = GetInventory(avatarInventory.source);
		if (avatarInventory.inventoryIndex >= inventory.size())
		{
			avatarInventory.inventoryIndex = 0;
		}

		size_t index = 0;
		for (auto& entry : inventory)
		{
			std::stringstream ss;
			ss << game::item::GetDescriptor(entry.first).name << " x " << entry.second;
			auto color = (index == avatarInventory.inventoryIndex) ? (avatarInventory.activeColor) : (avatarInventory.inactiveColor);
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
				model[common::data::Properties::WIDTH],
				model[visuals::data::Properties::ROW_HEIGHT],
				model[visuals::data::Properties::FONT],
				model[visuals::data::Properties::COLORS][visuals::data::Properties::INACTIVE],
				model[visuals::data::Properties::COLORS][visuals::data::Properties::ACTIVE],
				model[visuals::data::Properties::DROP_SHADOW],
				common::XY<int>(model[visuals::data::Properties::DROP_SHADOW_X],model[visuals::data::Properties::DROP_SHADOW_Y]),
				model[visuals::data::Properties::DROP_SHADOW_COLOR],
				0u,
				(InventorySource)(int)model[visuals::data::Properties::SOURCE]
			});
		if (model.count(visuals::data::Properties::CONTROL_ID) > 0)
		{
			avatarInventoryTable[layoutName][model[visuals::data::Properties::CONTROL_ID]] = index;
		}
		return [index](std::shared_ptr<SDL_Renderer> renderer)
		{
			DrawInternalAvatarInventory(renderer, index);
		};
	}

	void ResetIndex(const std::string& layoutName, const std::string& controlId)
	{
		SetInventoryIndex(layoutName, controlId, 0);
	}

	static std::map<int, size_t> GetInventory(const std::string& layoutName, const std::string& controlId)
	{
		return GetInventory(internalAvatarInventories[avatarInventoryTable.find(layoutName)->second.find(controlId)->second].source);
	}

	void NextIndex(const std::string& layoutName, const std::string& controlId)
	{
		auto inventory = GetInventory(layoutName, controlId);
		if (!inventory.empty())
		{
			SetInventoryIndex(layoutName, controlId, (GetInventoryIndex(layoutName, controlId) + 1) % inventory.size());
		}
	}

	void PreviousIndex(const std::string& layoutName, const std::string& controlId)
	{
		auto inventory = GetInventory(layoutName, controlId);
		if (!inventory.empty())
		{
			SetInventoryIndex(layoutName, controlId, (GetInventoryIndex(layoutName, controlId) + inventory.size() - 1) % inventory.size());
		}
	}

	std::optional<int> GetItem(const std::string& layoutName, const std::string& controlId)
	{
		auto inventory = GetInventory(layoutName, controlId);
		if (inventory.size() == 0)
		{
			return std::nullopt;
		}
		else if (GetInventoryIndex(layoutName, controlId) < inventory.size())
		{
			auto iter = inventory.begin();
			for (size_t dummy = 0; dummy < GetInventoryIndex(layoutName, controlId); ++dummy)
			{
				iter++;
			}
			return iter->first;
		}
		else
		{
			SetInventoryIndex(layoutName, controlId, 0);
			return GetItem(layoutName, controlId);
		}
	}

	void OnMouseMotion(const std::string& layoutName, const std::string& controlId, const common::XY<Sint32>& xy)
	{
		auto avatarInventoryIndex = avatarInventoryTable[layoutName][controlId];
		auto& avatarInventory = internalAvatarInventories[avatarInventoryIndex];
		
		int x = avatarInventory.xy.GetX();
		int y = avatarInventory.xy.GetY();
		int width = avatarInventory.width;
		int rowHeight = avatarInventory.rowHeight;
		if (xy.GetX() >= x && xy.GetX()<x+width && xy.GetY()>=y)
		{
			size_t row = ((size_t)xy.GetY() - (size_t)y) / (size_t)rowHeight;
			auto inventory = GetInventory(layoutName, controlId);
			if ((size_t)row < inventory.size())
			{
				SetInventoryIndex(layoutName, controlId, row);
			}
		}
	}

	std::optional<int> OnMouseButtonUp(const std::string& layoutName, const std::string& controlId, const common::XY<Sint32>& xy, Uint8 buttons)
	{
		auto avatarInventoryIndex = avatarInventoryTable[layoutName][controlId];
		auto& avatarInventory = internalAvatarInventories[avatarInventoryIndex];
		int x = avatarInventory.xy.GetX();
		int y = avatarInventory.xy.GetY();
		int width = avatarInventory.width;
		int rowHeight = avatarInventory.rowHeight;
		if (xy.GetX() >= x && xy.GetX() < x + width && xy.GetY() >= y)
		{
			size_t row = ((size_t)xy.GetY() - (size_t)y) / (size_t)rowHeight;
			auto inventory = GetInventory(layoutName, controlId);
			if ((size_t)row < inventory.size())
			{
				return std::optional<int>((int)row);
			}
		}
		return std::optional<int>();
	}

	std::function<void()> DoIndexReset(const std::string& layoutName, const std::string& controlId)
	{
		return [layoutName, controlId]() 
		{
			ResetIndex(layoutName, controlId);
		};
	}

	std::function<void()> GoToNextIndex(const std::string& layoutName, const std::string& controlId)
	{
		return [layoutName, controlId]()
		{
			NextIndex(layoutName, controlId);
		};
	}

	std::function<void()> GoToPreviousIndex(const std::string& layoutName, const std::string& controlId)
	{
		return [layoutName, controlId]()
		{
			PreviousIndex(layoutName, controlId);
		};
	}
}