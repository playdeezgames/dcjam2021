#pragma once
#include "Maze.Direction.h"
#include "Common.XY.h"
#include <string>
#include <optional>
#include "json.hpp"
namespace game::Avatar
{
	std::optional<std::string> TurnLeft();
	std::optional<std::string> TurnRight();
	std::optional<std::string> MoveAhead();
	std::optional<std::string> MoveBack();
	maze::Direction GetFacing();
	common::XY<size_t> GetPosition();
	void SetPosition(const common::XY<size_t>&);
	nlohmann::json& GetAvatar();//TODO: this is part of the DAL
}