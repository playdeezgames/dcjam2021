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
	const std::string FILENAME_AUTOSAVE = "autosave.json";
	const std::vector<std::string> FILENAME_SLOTS = 
	{
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

	void Reset()
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
		Reset();
	}

	void AutoSave()
	{
		data::JSON::Save(FILENAME_AUTOSAVE, GetData());
	}

	void LoadFromAutosave()
	{
		data = data::JSON::Load(FILENAME_AUTOSAVE);
	}

	bool DoesAutosaveExist()
	{
		return common::Utility::FileExists(FILENAME_AUTOSAVE);
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