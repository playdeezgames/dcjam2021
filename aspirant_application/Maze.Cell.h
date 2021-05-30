#pragma once
#include <map>
#include <vector>
#include <optional>
#include <memory>
#include <functional>
#include "Maze.Door.h"
namespace maze
{
	template<typename TDirection>
	class Cell
	{
	private:
		std::map<TDirection, std::shared_ptr<Door>> exits;
		std::map<TDirection, std::shared_ptr<Cell>> neighbors;
	public:
		Cell() 
			: exits()
			, neighbors()
		{

		}
		void PlaceNeighbor(TDirection direction, std::shared_ptr<Cell<TDirection>> cell)
		{
			neighbors[direction] = cell;
		}
		void PlaceDoor(TDirection direction, std::shared_ptr<Door> door)
		{
			exits[direction] = door;
		}
		bool SetDoor(TDirection direction, Door door)
		{
			auto iter = exits.find(direction);
			if (iter != exits.end())
			{
				(*iter->second) = door;
				return true;
			}
			return false;
		}
		std::optional<const std::shared_ptr<Cell<TDirection>>> GetNeighbor(TDirection direction) const
		{
			auto neighbor = neighbors.find(direction);
			return
				(neighbor != neighbors.end()) ?
				(std::make_optional(neighbor->second)) :
				(std::nullopt);
		}
		std::optional<std::shared_ptr<Cell<TDirection>>> GetNeighbor(TDirection direction)
		{
			auto neighbor = neighbors.find(direction);
			return
				(neighbor != neighbors.end()) ?
				(std::make_optional(neighbor->second)) :
				(std::nullopt);
		}
		std::optional<const std::shared_ptr<Door>> GetDoor(TDirection direction) const
		{
			auto door = exits.find(direction);
			return
				(door != exits.end()) ?
				(std::optional<const std::shared_ptr<Door>>(door->second)) :
				(std::nullopt);
		}
		std::optional<std::shared_ptr<Door>> GetDoor(TDirection direction)
		{
			auto door = exits.find(direction);
			return
				(door != exits.end()) ?
				(std::optional<std::shared_ptr<Door>>(door->second)) :
				(std::nullopt);
		}
		size_t CountDoorsThat(std::function<bool(const Door&)> condition) const
		{
			size_t count = 0u;
			for (auto& entry : exits)
			{
				if(condition(*entry.second))
				{
					count++;
				}
			}
			return count;
		}
	};
}