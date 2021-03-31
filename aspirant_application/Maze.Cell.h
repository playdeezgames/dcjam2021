#pragma once
#include "Maze.Direction.h"
#include "Maze.Door.h"
#include <map>
#include <vector>
#include <optional>
namespace maze
{
	class Cell
	{
	private:
		std::map<Direction, Door*> exits;
		std::map<Direction, Cell*> neighbors;
	public:
		Cell();
		void SetNeighbor(Direction, Cell*);
		void SetDoor(Direction, Door*);
		std::optional<const Cell*> GetNeighbor(Direction) const;
		std::optional<Cell*> GetNeighbor(Direction);
		std::optional<const Door*> GetDoor(Direction) const;
		std::optional<Door*> GetDoor(Direction);
		bool IsDeadEnd() const;
	};
}