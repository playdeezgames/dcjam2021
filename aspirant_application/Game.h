#pragma once
#include "json.hpp"
namespace game
{
	enum class Difficulty
	{
		EASY,
		NORMAL,
		HARD
	};
	void Start();
	void Reset(const Difficulty&);
	nlohmann::json& GetData();
	void AutoSave();
	bool DoesAutosaveExist();
	void LoadFromAutosave();
	bool DoesSlotExist(int);
	void LoadFromSlot(int);
	void SaveToSlot(int);
}
