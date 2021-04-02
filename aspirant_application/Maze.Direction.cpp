#include "Maze.Direction.h"
#include <string>
namespace maze
{
	std::vector<Direction> allDirections = 
	{
		Direction::NORTH,
		Direction::EAST,
		Direction::SOUTH,
		Direction::WEST
	};

	const std::vector<Direction>& Directions::All()
	{
		return allDirections;
	}



	Direction Directions::Opposite(Direction direction)
	{
		switch (direction)
		{
		case Direction::EAST:
			return Direction::WEST;
		case Direction::WEST:
			return Direction::EAST;
		case Direction::NORTH:
			return Direction::SOUTH;
		case Direction::SOUTH:
			return Direction::NORTH;
		default:
			throw std::string("bad direction");
		}
	}

	int Directions::NextColumn(int column, int row, Direction direction)
	{
		switch (direction)
		{
		case Direction::EAST:
			return column + 1;
		case Direction::WEST:
			return column - 1;
		case Direction::NORTH:
			return column;
		case Direction::SOUTH:
			return column;
		default:
			throw std::string("bad direction");
		}
	}

	int Directions::NextRow(int column, int row, Direction direction)
	{
		switch (direction)
		{
		case Direction::EAST:
			return row;
		case Direction::WEST:
			return row;
		case Direction::NORTH:
			return row - 1;
		case Direction::SOUTH:
			return row + 1;
		default:
			throw std::string("bad direction");
		}
	}
}