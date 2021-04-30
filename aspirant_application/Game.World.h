#pragma once
#include "Common.XY.h"
#include "Maze.Direction.h"
#include <string>
namespace game::world
{
	enum class Border
	{
		WALL,
		DOOR,
		LOCK
	};
}
namespace game::World
{
	world::Border GetBorderAhead(const common::XY<size_t>&, const maze::Direction&);
	world::Border GetBorderLeft(const common::XY<size_t>&, const maze::Direction&);
	world::Border GetBorderRight(const common::XY<size_t>&, const maze::Direction&);
	common::XY<size_t> GetSize();
	std::string XYToRoomKey(const common::XY<size_t>& xy);

	//TODO: is this more involved with avatar or world?
	//TODO: there will be an additional "known" state, in addition to the "visited" state
	bool IsExplored(const common::XY<size_t>&);
	void SetExplored(const common::XY<size_t>&);
	bool IsExitable(const common::XY<size_t>&);
	bool CanSpawnAvatar(const common::XY<size_t>&);
}