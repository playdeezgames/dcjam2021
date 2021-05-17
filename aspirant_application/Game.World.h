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
namespace game::world::Borders
{
	world::Border GetBorderAhead(const common::XY<size_t>&, const maze::Direction&);
	void SetBorderAhead(const common::XY<size_t>&, const maze::Direction&, const world::Border&);
	world::Border GetBorderLeft(const common::XY<size_t>&, const maze::Direction&);
	world::Border GetBorderRight(const common::XY<size_t>&, const maze::Direction&);
	bool IsExitable(const common::XY<size_t>&);
	std::vector<common::XY<size_t>> GetDeadEnds();
	bool CanSpawnAvatar(const common::XY<size_t>&);
}

namespace game::world
{
	enum class KnownState
	{
		UNKNOWN,
		KNOWN,
		VISITED
	};
}
namespace game::world::Data
{
	std::string XYToRoomKey(const common::XY<size_t>& xy);
}
namespace game::World
{
	common::XY<size_t> GetSize();
	game::world::KnownState GetKnownState(const common::XY<size_t>&);
	void SetExplored(const common::XY<size_t>&);
	void SetKnown(const common::XY<size_t>&);
}