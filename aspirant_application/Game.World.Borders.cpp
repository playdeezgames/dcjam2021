#include "Game.World.h"
#include <vector>
#include "Maze.h"
#include "Common.RNG.h"
#include "Maze.Direction.h"
#include "Game.h"
#include <sstream>
#include "Game.Data.Properties.h"
#include "Data.Stores.h"
#include "Game.Creatures.h"
#include <optional>
#include "Game.World.Borders.h"
#include "Game.World.Data.h"

#include "Game.Shoppes.h"
namespace game::world::Borders
{
	const size_t NS_BORDER_COUNT = game::World::ROWS * game::World::COLUMNS + game::World::COLUMNS;
	const size_t NS_BORDER_STRIDE = game::World::COLUMNS;
	const size_t EW_BORDER_COUNT = game::World::ROWS * game::World::COLUMNS + game::World::ROWS;
	const size_t EW_BORDER_STRIDE = game::World::COLUMNS + 1;

	static size_t XYToNorthBorderIndex(const common::XY<size_t> position)
	{
		return position.GetX() + position.GetY() * NS_BORDER_STRIDE;
	}

	static size_t XYToSouthBorderIndex(const common::XY<size_t> position)
	{
		return XYToNorthBorderIndex(position) + NS_BORDER_STRIDE;
	}

	static size_t XYToWestBorderIndex(const common::XY<size_t> position)
	{
		return position.GetX() + position.GetY() * EW_BORDER_STRIDE;
	}

	static size_t XYToEastBorderIndex(const common::XY<size_t> position)
	{
		return XYToWestBorderIndex(position) + 1;
	}

	static std::string IndexToString(size_t index)
	{
		std::stringstream ss;
		ss << index;
		return ss.str();
	}

	static void SetNSBorder(size_t index, world::Border border)
	{
		auto& borders = game::world::Data::GetNSBorders();
		borders[IndexToString(index)] = (int)border;
	}

	static world::Border GetNSBorder(size_t index)
	{
		auto key = IndexToString(index);
		auto& borders = game::world::Data::GetNSBorders();
		if (borders.count(key) > 0)
		{
			return (world::Border)(int)borders[key];
		}
		return world::Border::WALL;
	}

	static void SetEWBorder(size_t index, world::Border border)
	{
		auto& borders = game::world::Data::GetEWBorders();
		borders[IndexToString(index)] = (int)border;
	}

	static world::Border GetEWBorder(size_t index)
	{
		auto key = IndexToString(index);
		auto& borders = game::world::Data::GetEWBorders();
		if (borders.count(key) > 0)
		{
			return (world::Border)(int)borders[key];
		}
		return world::Border::WALL;
	}

	world::Border GetBorderAhead(const common::XY<size_t>& position, const maze::Direction& direction)
	{
		switch (direction)
		{
		case maze::Direction::NORTH:
			return GetNSBorder(XYToNorthBorderIndex(position));
		case maze::Direction::EAST:
			return GetEWBorder(XYToEastBorderIndex(position));
		case maze::Direction::SOUTH:
			return GetNSBorder(XYToSouthBorderIndex(position));
		default:
			return GetEWBorder(XYToWestBorderIndex(position));
		}
	}

	void SetBorderAhead(const common::XY<size_t>& position, const maze::Direction& direction, const world::Border& border)
	{
		switch (direction)
		{
		case maze::Direction::NORTH:
			SetNSBorder(XYToNorthBorderIndex(position), border);
			break;
		case maze::Direction::EAST:
			SetEWBorder(XYToEastBorderIndex(position), border);
			break;
		case maze::Direction::SOUTH:
			SetNSBorder(XYToSouthBorderIndex(position), border);
			break;
		default:
			SetEWBorder(XYToWestBorderIndex(position), border);
			break;
		}
	}

	world::Border GetBorderLeft(const common::XY<size_t>& position, const maze::Direction& direction)
	{
		switch (direction)
		{
		case maze::Direction::NORTH:
			return GetEWBorder(XYToWestBorderIndex(position));
		case maze::Direction::EAST:
			return GetNSBorder(XYToNorthBorderIndex(position));
		case maze::Direction::SOUTH:
			return GetEWBorder(XYToEastBorderIndex(position));
		default:
			return GetNSBorder(XYToSouthBorderIndex(position));
		}
	}

	world::Border GetBorderRight(const common::XY<size_t>& position, const maze::Direction& direction)
	{
		switch (direction)
		{
		case maze::Direction::NORTH:
			return GetEWBorder(XYToEastBorderIndex(position));
		case maze::Direction::EAST:
			return GetNSBorder(XYToSouthBorderIndex(position));
		case maze::Direction::SOUTH:
			return GetEWBorder(XYToWestBorderIndex(position));
		default:
			return GetNSBorder(XYToNorthBorderIndex(position));
		}
	}

	void UpdateBorders(const maze::Maze& maze)
	{
		for (auto column = 0; column < maze.GetColumns(); ++column)
		{
			for (auto row = 0; row < maze.GetRows(); ++row)
			{
				size_t nsBorderIndex = column + row * NS_BORDER_STRIDE;
				size_t ewBorderIndex = column + row * EW_BORDER_STRIDE;

				auto cell = maze.GetCell((int)column, (int)row);
				auto northDoor = cell.value()->GetDoor(maze::Direction::NORTH);
				if (northDoor && *northDoor.value() == maze::Door::OPEN)
				{
					auto nextCell = cell.value()->GetNeighbor(maze::Direction::NORTH);
					if (cell.value()->IsDeadEnd() || nextCell.value()->IsDeadEnd())
					{
						SetNSBorder(nsBorderIndex, world::Border::LOCK);
					}
					else
					{
						SetNSBorder(nsBorderIndex, world::Border::DOOR);
					}
				}
				else
				{
					SetNSBorder(nsBorderIndex, world::Border::WALL);
				}
				auto westDoor = cell.value()->GetDoor(maze::Direction::WEST);
				if (westDoor && *westDoor.value() == maze::Door::OPEN)
				{
					auto nextCell = cell.value()->GetNeighbor(maze::Direction::WEST);
					if (cell.value()->IsDeadEnd() || nextCell.value()->IsDeadEnd())
					{
						SetEWBorder(ewBorderIndex, world::Border::LOCK);
					}
					else
					{
						SetEWBorder(ewBorderIndex, world::Border::DOOR);
					}
				}
				else
				{
					SetEWBorder(ewBorderIndex, world::Border::WALL);
				}
			}
		}
	}

	bool IsExitable(const common::XY<size_t>& position)
	{
		return
			GetNSBorder(XYToNorthBorderIndex(position)) == game::world::Border::DOOR ||
			GetNSBorder(XYToSouthBorderIndex(position)) == game::world::Border::DOOR ||
			GetEWBorder(XYToEastBorderIndex(position)) == game::world::Border::DOOR ||
			GetEWBorder(XYToWestBorderIndex(position)) == game::world::Border::DOOR;
	}

	bool CanSpawnAvatar(const common::XY<size_t>& position)
	{
		//return game::Shoppes::Read(position).has_value();
		return !game::Creatures::GetInstance(position) && IsExitable(position);
	}

	std::vector<common::XY<size_t>> GetDeadEnds()
	{
		std::vector<common::XY<size_t>> result;
		auto worldSize = game::World::GetSize();
		for (size_t x = 0u; x < worldSize.GetX(); ++x)
		{
			for (size_t y = 0u; y < worldSize.GetY(); ++y)
			{
				if (!IsExitable({ x,y }))
				{
					result.push_back({ x,y });
				}
			}
		}
		return result;
	}
}
