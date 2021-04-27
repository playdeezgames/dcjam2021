#pragma once
#include "json.hpp"
namespace game
{
	void Start();
	void Reset();
	nlohmann::json& GetData();
	void AutoSave();
	bool DoesAutosaveExist();
	void LoadFromAutosave();
	bool DoesSlotExist(int);
	void LoadFromSlot(int);
}
