#include <sstream>
#include "Common.RNG.h"
#include "Game.h"
#include "Game.Avatar.h"
#include "Game.Data.Properties.h"
#include "Game.Shoppes.h"
#include "Game.World.h"
#include "Game.World.Borders.h"
#include "Game.World.Data.h"
#include "Game.World.Items.h"
namespace game::world::items::Data
{
	const std::string ROOM_INVENTORIES = "room-inventories";

	static nlohmann::json& RoomInventories()
	{
		auto& data = game::GetData();
		if (data.count(ROOM_INVENTORIES) == 0)
		{
			data[ROOM_INVENTORIES] = nlohmann::json();
		}
		return data[ROOM_INVENTORIES];
	}

	static nlohmann::json& RoomInventory(const common::XY<size_t>& xy)
	{
		auto roomKey = game::world::Data::XYToRoomKey(xy);
		auto& inventories = RoomInventories();
		if (inventories.count(roomKey) == 0)
		{
			inventories[roomKey] = nlohmann::json();
		}
		return inventories[roomKey];
	}

	static void Clear()
	{
		RoomInventories().clear();
	}
}
namespace game::world::Items
{

	static bool TryPopulateItem(int item)
	{
		auto worldSize = game::World::GetSize();
		common::XY<size_t> position = { common::RNG::FromRange(0u, worldSize.GetX()), common::RNG::FromRange(0u, worldSize.GetY()) };
		if (!game::Shoppes::Read(position))
		{
			Add(position, item, 1);
			return true;
		}
		return false;
	}

	static void PopulateItems(const game::Difficulty& difficulty)
	{
		for (auto item : game::item::All())
		{
			auto& descriptor = game::item::GetDescriptor(item);
			auto count = descriptor.numberAppearing[(int)difficulty];
			while (count > 0)
			{
				if (TryPopulateItem(item))
				{
					count--;
				}
			}
		}
	}

	static void PopulateKeys()
	{
		int keyItem = game::Avatar::GetDescriptor().keyItemId;
		auto worldSize = game::World::GetSize();
		auto deadEnds = game::world::Borders::GetDeadEnds([](const common::XY<size_t>&) { return true; });
		size_t keyCount = deadEnds.size();
		while (keyCount > 0)
		{
			common::XY<size_t> location =
			{
				common::RNG::FromRange(0u, worldSize.GetX()) , 
				common::RNG::FromRange(0u, worldSize.GetY()) 
			};
			auto iter = std::find(deadEnds.begin(), deadEnds.end(), location);
			if (iter == deadEnds.end())
			{
				Add(location, keyItem, 1);
				keyCount--;
			}
		}
	}

	void Reset(const game::Difficulty& difficulty)
	{
		items::Data::Clear();
		PopulateItems(difficulty);
		PopulateKeys();
	}

	static std::string IntToItemKey(const int& item)
	{
		std::stringstream ss;
		ss << item;
		return ss.str();
	}

	size_t GetRoomInventory(const common::XY<size_t>& location, const int& item)
	{
		auto& roomInventory = items::Data::RoomInventory(location);
		if (roomInventory.count(IntToItemKey(item)) > 0)
		{
			return roomInventory[IntToItemKey(item)];
		}
		return 0;
	}

	static void SetRoomInventory(const common::XY<size_t>& location, const int& item, size_t amount)
	{
		auto itemKey = IntToItemKey(item);
		auto& roomInventory = items::Data::RoomInventory(location);
		roomInventory[itemKey] = amount;
	}

	bool AnyPresent(const common::XY<size_t>& location)
	{
		auto& roomInventory = items::Data::RoomInventory(location);
		for (auto& entry : roomInventory.items())
		{
			int count = entry.value();
			if (count > 0)
			{
				return true;
			}
		}
		return false;
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