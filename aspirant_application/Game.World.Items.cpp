#include "Game.World.Items.h"
#include <map>
#include <vector>
#include "Common.RNG.h"
#include "Game.World.h"
#include "Game.Item.h"
#include "Game.h"
#include "Game.Properties.h"
#include <sstream>
namespace game::world::Items
{
	static nlohmann::json& GetRoomInventories()
	{
		auto& data = game::GetData();
		if (data.count(game::Properties::ROOM_INVENTORIES) == 0)
		{
			data[game::Properties::ROOM_INVENTORIES] = nlohmann::json();
		}
		return data[game::Properties::ROOM_INVENTORIES];
	}

	static std::string XYToRoomKey(const common::XY<size_t>& xy)//TODO: this is duplicated code
	{
		std::stringstream ss;
		ss << "(" << xy.GetX() << "," << xy.GetY() << ")";
		return ss.str();
	}

	static nlohmann::json& GetRoomInventory(const common::XY<size_t>& xy)
	{
		auto roomKey = XYToRoomKey(xy);
		auto& inventories = GetRoomInventories();
		if (inventories.count(roomKey) == 0)
		{
			inventories[roomKey] = nlohmann::json();
		}
		return inventories[roomKey];
	}

	static void PopulateItem(int item)
	{
		auto worldSize = game::World::GetSize();
		size_t column = (size_t)common::RNG::FromRange(0, (int)worldSize.GetX());
		size_t row = (size_t)common::RNG::FromRange(0, (int)worldSize.GetY());
		Add({column, row}, item, 1);
	}

	static void PopulateItems()
	{
		size_t totalCount = 0;
		for (auto item : game::item::All())
		{
			auto descriptor = game::item::GetDescriptor(item);
			auto count = descriptor.numberAppearing;
			while (count > 0)
			{
				totalCount++;
				PopulateItem(item);
				count--;
			}
		}
		totalCount = totalCount;
	}

	void Reset()
	{
		GetRoomInventories().clear();
		PopulateItems();
	}

	static std::string IntToItemKey(const int& item)
	{
		std::stringstream ss;
		ss << item;
		return ss.str();
	}

	static size_t GetRoomInventory(const common::XY<size_t>& location, const int& item)
	{
		auto& roomInventory = GetRoomInventory(location);
		if (roomInventory.count(IntToItemKey(item)) > 0)
		{
			return roomInventory[IntToItemKey(item)];
		}
		return 0;
	}

	static void SetRoomInventory(const common::XY<size_t>& location, const int& item, size_t amount)
	{
		auto& roomInventory = GetRoomInventory(location);
		roomInventory[IntToItemKey(item)] = amount;
	}

	bool IsPresent(const common::XY<size_t>& location, const int& item)
	{
		return GetRoomInventory(location, item) > 0;
	}

	std::map<int, size_t> FloorInventory(const common::XY<size_t>& location)
	{
		std::map<int, size_t> result;
		for (auto item : game::item::All())
		{
			auto amount = GetRoomInventory(location, item);
			if (amount > 0)
			{
				result[item] = amount;
			}
		}
		return result;
	}

	size_t Remove(const common::XY<size_t>& location, const int& item, size_t quantity)
	{
		if (IsPresent(location, item))
		{
			size_t total = GetRoomInventory(location, item);
			if (quantity >= total)
			{
				quantity = total;
				SetRoomInventory(location, item, 0);
			}
			else
			{
				SetRoomInventory(location, item, GetRoomInventory(location, item) - quantity);
			}
			return quantity;
		}
		else
		{
			return 0;
		}
	}

	void Add(const common::XY<size_t>& location, const int& item, size_t amount)
	{
		if (amount > 0)
		{
			SetRoomInventory(location, item, GetRoomInventory(location, item) + amount);
		}
	}
}