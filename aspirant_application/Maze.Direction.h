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
	namespace Directions
	{
		int NextColumn(int, int, const Direction&);
		int NextRow(int, int, const Direction&);
		const std::vector<Direction>& All();
		Direction Opposite(const Direction&);
	}
}