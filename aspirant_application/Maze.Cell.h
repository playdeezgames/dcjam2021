#pragma once
#include <map>
#include <vector>
#include <optional>
#include <memory>
#include <functional>
namespace maze
{
	template<typename TDirection, typename TDoor>
	class Cell
	{
	private:
		std::map<TDirection, std::shared_ptr<TDoor>> exits;
		std::map<TDirection, std::shared_ptr<Cell<TDirection, TDoor>>> neighbors;
	public:
		Cell() 
			: exits()
			, neighbors()
		{

		}
		void PlaceNeighbor(TDirection direction, std::shared_ptr<Cell<TDirection, TDoor>> cell)
		{
			neighbors[direction] = cell;
		}
		void PlaceDoor(TDirection direction, std::shared_ptr<TDoor> door)
		{
			exits[direction] = door;
		}
		bool SetDoor(TDirection direction, TDoor door)
		{
			auto iter = exits.find(direction);
			if (iter != exits.end())
			{
				(*iter->second) = door;
				return true;
			}
			return false;
		}
		std::optional<const std::shared_ptr<Cell<TDirection, TDoor>>> GetNeighbor(TDirection direction) const
		{
			auto neighbor = neighbors.find(direction);
			return
				(neighbor != neighbors.end()) ?
				(std::make_optional(neighbor->second)) :
				(std::nullopt);
		}
		std::optional<std::shared_ptr<Cell<TDirection, TDoor>>> GetNeighbor(TDirection direction)
		{
			auto neighbor = neighbors.find(direction);
			return
				(neighbor != neighbors.end()) ?
				(std::make_optional(neighbor->second)) :
				(std::nullopt);
		}
		std::optional<const std::shared_ptr<TDoor>> GetDoor(TDirection direction) const
		{
			auto door = exits.find(direction);
			return
				(door != exits.end()) ?
				(std::optional<const std::shared_ptr<TDoor>>(door->second)) :
				(std::nullopt);
		}
		std::optional<std::shared_ptr<TDoor>> GetDoor(TDirection direction)
		{
			auto door = exits.find(direction);
			return
				(door != exits.end()) ?
				(std::optional<std::shared_ptr<TDoor>>(door->second)) :
				(std::nullopt);
		}
		size_t CountDoorsThat(std::function<bool(const TDoor&)> condition) const
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