#pragma once
#include "Maze.Direction.h"
#include "Common.XY.h"
namespace game
{
	enum class Border
	{
		WALL,
		DOOR
	};
	Border GetLeftSide();
	Border GetAhead();
	Border GetRightSide();
	void TurnLeft();
	void TurnRight();
	void MoveAhead();
	maze::Direction GetFacing();
	common::XY<size_t> GetPosition();

	void Start();
}
