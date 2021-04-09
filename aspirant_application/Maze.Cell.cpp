#include "Maze.Cell.h"
namespace maze
{
	Cell::Cell()
		: exits()
		, neighbors()
	{

	}

	void Cell::SetNeighbor(Direction direction, std::shared_ptr<Cell> cell)
	{
		neighbors[direction] = cell;
	}

	void Cell::SetDoor(Direction direction, std::shared_ptr<Door> door)
	{
		exits[direction] = door;
	}

	std::optional<const std::shared_ptr<Cell>> Cell::GetNeighbor(Direction direction) const
	{
		auto neighbor = neighbors.find(direction);
		return 
			(neighbor != neighbors.end()) ?
			(std::make_optional(neighbor->second)) :
			(std::nullopt);
	}

	std::optional<std::shared_ptr<Cell>> Cell::GetNeighbor(Direction direction)
	{
		auto neighbor = neighbors.find(direction);
		return
			(neighbor != neighbors.end()) ?
			(std::make_optional(neighbor->second)) :
			(std::nullopt);
	}

	std::optional<const std::shared_ptr<Door>> Cell::GetDoor(Direction direction) const
	{
		auto door = exits.find(direction);
		return 
			(door!=exits.end()) ? 
			(std::optional<const std::shared_ptr<Door>>(door->second)) :
			(std::nullopt);
	}

	std::optional<std::shared_ptr<Door>> Cell::GetDoor(Direction direction)
	{
		auto door = exits.find(direction);
		return
			(door != exits.end()) ?
			(std::optional<std::shared_ptr<Door>>(door->second)) :
			(std::nullopt);
	}

	bool Cell::IsDeadEnd() const
	{
		int count = 0;
		for (auto& entry : exits)
		{
			if (*entry.second == Door::OPEN)
			{
				count++;
			}
		}
		return count == 1;
	}

}