#pragma once
#include "Common.XY.h"
#include "Maze.Direction.h"
#include "Game.World.Border.h"
namespace game::World
{
	world::Border GetLeftSide();
	world::Border GetAhead();
	world::Border GetRightSide();

	common::XY<size_t> GetSize();
	world::Border GetBorder(const common::XY<size_t>&, const maze::Direction&);
	bool IsExplored(const common::XY<size_t>&);
	void SetExplored(const common::XY<size_t>&);
}