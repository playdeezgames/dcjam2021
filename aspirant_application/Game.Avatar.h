#pragma once
#include "Maze.Direction.h"
#include "Common.XY.h"
namespace game::Avatar
{
	void TurnLeft();
	void TurnRight();
	void MoveAhead();
	maze::Direction GetFacing();
	common::XY<size_t> GetPosition();
	bool IsExplored(const common::XY<size_t>&);
}