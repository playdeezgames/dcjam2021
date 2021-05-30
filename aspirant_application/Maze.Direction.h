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
		size_t NextColumn(size_t, size_t, const Direction&);
		size_t NextRow(size_t, size_t, const Direction&);
		const std::vector<Direction>& All();
		Direction Opposite(const Direction&);
	}
}