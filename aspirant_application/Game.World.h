#pragma once
#include "Common.XY.h"
#include "Maze.Direction.h"
#include <string>
#include "json.hpp"
#include "Maze.h"

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
	void UpdateBorders(const maze::Maze& maze);
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
	nlohmann::json& GetNSBorders();
	nlohmann::json& GetEWBorders();
}
namespace game::World
{
	const size_t COLUMNS = 12;
	const size_t ROWS = 12;
	const size_t NS_BORDER_COUNT = ROWS * COLUMNS + COLUMNS;
	const size_t NS_BORDER_STRIDE = COLUMNS;
	const size_t EW_BORDER_COUNT = ROWS * COLUMNS + ROWS;
	const size_t EW_BORDER_STRIDE = COLUMNS + 1;
	common::XY<size_t> GetSize();
	game::world::KnownState GetKnownState(const common::XY<size_t>&);
	void SetExplored(const common::XY<size_t>&);
	void SetKnown(const common::XY<size_t>&);
}