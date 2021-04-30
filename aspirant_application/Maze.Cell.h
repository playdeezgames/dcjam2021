#pragma once
#include "Maze.Direction.h"
#include <map>
#include <vector>
#include <optional>
#include <memory>
namespace maze
{
	enum class Door
	{
		CLOSED,
		OPEN
	};

	class Cell
	{
	private:
		std::map<Direction, std::shared_ptr<Door>> exits;
		std::map<Direction, std::shared_ptr<Cell>> neighbors;
	public:
		Cell();
		void PlaceNeighbor(Direction, std::shared_ptr<Cell>);
		void PlaceDoor(Direction, std::shared_ptr<Door>);
		bool SetDoor(Direction, Door);
		std::optional<const std::shared_ptr<Cell>> GetNeighbor(Direction) const;
		std::optional<std::shared_ptr<Cell>> GetNeighbor(Direction);
		std::optional<const std::shared_ptr<Door>> GetDoor(Direction) const;
		std::optional<std::shared_ptr<Door>> GetDoor(Direction);
		bool IsDeadEnd() const;
	};
}