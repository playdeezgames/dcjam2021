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
		std::map<Direction, Door*> exits;//TODO: shared_ptr
		std::map<Direction, Cell*> neighbors;//TODO: shared_ptr
	public:
		Cell();
		void SetNeighbor(Direction, Cell*);//TODO: shared_ptr
		void SetDoor(Direction, Door*);//TODO: shared_ptr
		std::optional<const Cell*> GetNeighbor(Direction) const;//TODO: shared_ptr
		std::optional<Cell*> GetNeighbor(Direction);//TODO: shared_ptr
		std::optional<const Door*> GetDoor(Direction) const;//TODO: shared_ptr
		std::optional<Door*> GetDoor(Direction);//TODO: shared_ptr
		bool IsDeadEnd() const;
	};
}