#pragma once
#include <vector>
namespace maze
{
	enum class Direction
	{
		NORTH,
		EAST,
		SOUTH,
		WEST
	};
	namespace Directions//TODO: gets its own file
	{
		int NextColumn(int, int, Direction);
		int NextRow(int, int, Direction);
		const std::vector<Direction>& All();
		Direction Opposite(Direction);
	}
}