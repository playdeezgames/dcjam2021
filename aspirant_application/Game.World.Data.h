#pragma once
#include <string>
#include "Common.XY.h"
#include "json.hpp"
namespace game::world::Data
{
	std::string XYToRoomKey(const common::XY<size_t>& xy);
	nlohmann::json& GetNSBorders();
	nlohmann::json& GetEWBorders();
	nlohmann::json& GetExplored();
	void ClearExplored();
}
