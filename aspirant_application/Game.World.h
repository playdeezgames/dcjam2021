#pragma once
#include "Common.XY.h"
#include "Maze.Direction.h"
#include "Game.World.Border.h"
namespace game::World
{
	world::Border GetBorderAhead(const common::XY<size_t>&, const maze::Direction&);
	world::Border GetBorderLeft(const common::XY<size_t>&, const maze::Direction&);
	world::Border GetBorderRight(const common::XY<size_t>&, const maze::Direction&);
	common::XY<size_t> GetSize();
	bool IsExplored(const common::XY<size_t>&);
	void SetExplored(const common::XY<size_t>&);
}