#pragma once
#include "Common.XY.h"
#include "Maze.Direction.h"
namespace game::World
{
	enum class Border
	{
		WALL,
		DOOR
	};
	Border GetLeftSide();
	Border GetAhead();
	Border GetRightSide();

	common::XY<size_t> GetSize();
	Border GetBorder(const common::XY<size_t>&, const maze::Direction &);
}