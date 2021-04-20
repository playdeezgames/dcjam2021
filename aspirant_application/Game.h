#pragma once
#include "json.hpp"
namespace game
{
	void Start();
	void Reset();
	nlohmann::json& GetData();//TODO: DAL
	void AutoSave();
}
