#pragma once
#include "Common.XY.h"
#include "Maze.h"
#include <functional>
namespace game::world
{
	enum class Border
	{
		WALL,
		DOOR,
		LOCK
	};
}
namespace game::world::Borders
{
	world::Border GetBorderAhead(const common::XY<size_t>&, const maze::Direction&);
	void SetBorderAhead(const common::XY<size_t>&, const maze::Direction&, const world::Border&);
	world::Border GetBorderLeft(const common::XY<size_t>&, const maze::Direction&);
	world::Border GetBorderRight(const common::XY<size_t>&, const maze::Direction&);
	bool IsExitable(const common::XY<size_t>&);
	std::vector<common::XY<size_t>> GetDeadEnds(std::function<bool(const common::XY<size_t>&)>);
	bool CanSpawnAvatar(const common::XY<size_t>&);
	void UpdateBorders(const maze::Maze& maze);
}
