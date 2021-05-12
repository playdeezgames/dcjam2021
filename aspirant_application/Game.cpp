#include "Game.h"
#include "json.hpp"
#include "Data.JSON.h"
#include "Common.Utility.h"
#include <vector>
namespace game::World { void Reset(); }
namespace game::Avatar { void Reset(); }
namespace game::avatar::Statistics { void Reset(); }
namespace game::world::Items { void Reset(); }
namespace game::avatar::Items { void Reset(); }
namespace game::Creatures { void Reset(); }
namespace game
{
	const std::vector<std::string> FILENAME_SLOTS = 
	{
		"autosave.json",
		"slot1.json",
		"slot2.json",
		"slot3.json",
		"slot4.json",
		"slot5.json"
	};

	nlohmann::json data;

	nlohmann::json& GetData()
	{
		return data;
	}

	void Reset(const Difficulty& difficulty)
	{
		game::World::Reset();
		game::world::Items::Reset();
		game::avatar::Statistics::Reset();
		game::avatar::Items::Reset();
		game::Creatures::Reset();
		game::Avatar::Reset();
	}

	void Start()
	{
		Reset(Difficulty::NORMAL);
	}

	void AutoSave()
	{
		data::JSON::Save(FILENAME_SLOTS[0], GetData());
	}

	void LoadFromAutosave()
	{
		data = data::JSON::Load(FILENAME_SLOTS[0]);
	}

	bool DoesAutosaveExist()
	{
		return common::Utility::FileExists(FILENAME_SLOTS[0]);
	}

	bool DoesSlotExist(int slot)
	{
		return common::Utility::FileExists(FILENAME_SLOTS[slot]);

	}

	void LoadFromSlot(int slot)
	{
		data = data::JSON::Load(FILENAME_SLOTS[slot]);
	}

	void SaveToSlot(int slot)
	{
		data::JSON::Save(FILENAME_SLOTS[slot], GetData());
	}

}