#include "Game.World.Borders.h"
#include "Game.World.h"
#include <sstream>
#include "Game.Creatures.h"
#include "Game.World.Data.h"
#include <map>
#include <functional>
namespace game::world::Borders
{
	const size_t NS_BORDER_COUNT = game::World::ROWS * game::World::COLUMNS + game::World::COLUMNS;
	const size_t NS_BORDER_STRIDE = game::World::COLUMNS;
	const size_t EW_BORDER_COUNT = game::World::ROWS * game::World::COLUMNS + game::World::ROWS;
	const size_t EW_BORDER_STRIDE = game::World::COLUMNS + 1;

	enum class IndexPlotter
	{
		NORTH,
		SOUTH,
		EAST,
		WEST
	};

	const std::map<IndexPlotter, std::function<size_t(const common::XY<size_t>&)>> indexPlotters =
	{
		{IndexPlotter::NORTH, [](const common::XY<size_t>& position) {return position.GetX() + position.GetY() * NS_BORDER_STRIDE; }},
		{IndexPlotter::SOUTH, [](const common::XY<size_t>& position) {return position.GetX() + position.GetY() * NS_BORDER_STRIDE + NS_BORDER_STRIDE; }},
		{IndexPlotter::EAST, [](const common::XY<size_t>& position) {return position.GetX() + position.GetY() * EW_BORDER_STRIDE + 1; }},
		{IndexPlotter::WEST, [](const common::XY<size_t>& position) {return position.GetX() + position.GetY() * EW_BORDER_STRIDE; }}
	};

	static std::string IndexToString(size_t index)
	{
		std::stringstream ss;
		ss << index;
		return ss.str();
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

	static void SetNSBorder(size_t index, world::Border border)
	{
		auto& borders = game::world::Data::GetNSBorders();
		borders[IndexToString(index)] = (int)border;
	}

	static void SetEWBorder(size_t index, world::Border border)
	{
		auto& borders = game::world::Data::GetEWBorders();
		borders[IndexToString(index)] = (int)border;
	}

	world::Border GetBorderAhead(const common::XY<size_t>& position, const maze::Direction& direction)
	{
		switch (direction)
		{
		case maze::Direction::NORTH:
			return GetNSBorder(indexPlotters.find(IndexPlotter::NORTH)->second(position));
		case maze::Direction::EAST:
			return GetEWBorder(indexPlotters.find(IndexPlotter::EAST)->second(position));
		case maze::Direction::SOUTH:
			return GetNSBorder(indexPlotters.find(IndexPlotter::SOUTH)->second(position));
		default:
			return GetEWBorder(indexPlotters.find(IndexPlotter::WEST)->second(position));
		}
	}

	void SetBorderAhead(const common::XY<size_t>& position, const maze::Direction& direction, const world::Border& border)
	{
		switch (direction)
		{
		case maze::Direction::NORTH:
			SetNSBorder(indexPlotters.find(IndexPlotter::NORTH)->second(position), border);
			break;
		case maze::Direction::EAST:
			SetEWBorder(indexPlotters.find(IndexPlotter::EAST)->second(position), border);
			break;
		case maze::Direction::SOUTH:
			SetNSBorder(indexPlotters.find(IndexPlotter::SOUTH)->second(position), border);
			break;
		default:
			SetEWBorder(indexPlotters.find(IndexPlotter::WEST)->second(position), border);
			break;
		}
	}

	world::Border GetBorderLeft(const common::XY<size_t>& position, const maze::Direction& direction)
	{
		switch (direction)
		{
		case maze::Direction::NORTH:
			return GetEWBorder(indexPlotters.find(IndexPlotter::WEST)->second(position));
		case maze::Direction::EAST:
			return GetNSBorder(indexPlotters.find(IndexPlotter::NORTH)->second(position));
		case maze::Direction::SOUTH:
			return GetEWBorder(indexPlotters.find(IndexPlotter::EAST)->second(position));
		default:
			return GetNSBorder(indexPlotters.find(IndexPlotter::SOUTH)->second(position));
		}
	}

	world::Border GetBorderRight(const common::XY<size_t>& position, const maze::Direction& direction)
	{
		switch (direction)
		{
		case maze::Direction::NORTH:
			return GetEWBorder(indexPlotters.find(IndexPlotter::EAST)->second(position));
		case maze::Direction::EAST:
			return GetNSBorder(indexPlotters.find(IndexPlotter::SOUTH)->second(position));
		case maze::Direction::SOUTH:
			return GetEWBorder(indexPlotters.find(IndexPlotter::WEST)->second(position));
		default:
			return GetNSBorder(indexPlotters.find(IndexPlotter::NORTH)->second(position));
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
			GetNSBorder(indexPlotters.find(IndexPlotter::NORTH)->second(position)) == game::world::Border::DOOR ||
			GetNSBorder(indexPlotters.find(IndexPlotter::SOUTH)->second(position)) == game::world::Border::DOOR ||
			GetEWBorder(indexPlotters.find(IndexPlotter::EAST)->second(position)) == game::world::Border::DOOR ||
			GetEWBorder(indexPlotters.find(IndexPlotter::WEST)->second(position)) == game::world::Border::DOOR;
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
