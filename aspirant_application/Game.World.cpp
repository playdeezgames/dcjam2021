#include "Game.World.h"
#include <vector>
#include "Maze.h"
#include "Common.RNG.h"
#include "Maze.Direction.h"
#include "Game.h"
#include <sstream>
#include "Game.Properties.h"
namespace game::World
{
	static std::string XYToRoomKey(const common::XY<size_t>& xy)//TODO: this is duplicated code
	{
		std::stringstream ss;
		ss << "(" << xy.GetX() << "," << xy.GetY() << ")";
		return ss.str();
	}

	static nlohmann::json& GetNSBorders()
	{
		auto& data = game::GetData();
		if (data.count(game::Properties::NS_BORDERS) == 0)
		{
			data[game::Properties::NS_BORDERS] = nlohmann::json();
		}
		return data[game::Properties::NS_BORDERS];
	}

	static nlohmann::json& GetEWBorders()
	{
		auto& data = game::GetData();
		if (data.count(game::Properties::EW_BORDERS) == 0)
		{
			data[game::Properties::EW_BORDERS] = nlohmann::json();
		}
		return data[game::Properties::EW_BORDERS];
	}

	static nlohmann::json& GetExplored()
	{
		auto& data = game::GetData();
		if (data.count(game::Properties::EXPLORED) == 0)
		{
			data[game::Properties::EXPLORED] = nlohmann::json();
		}
		return data[game::Properties::EXPLORED];
	}

	const size_t COLUMNS = 12;//TODO: make this configurable?
	const size_t ROWS = 12;//TODO: make this configurable?
	const size_t NS_BORDER_COUNT = ROWS * COLUMNS + COLUMNS;
	const size_t NS_BORDER_STRIDE = COLUMNS;
	const size_t EW_BORDER_COUNT = ROWS * COLUMNS + ROWS;
	const size_t EW_BORDER_STRIDE = COLUMNS + 1;

	static void SetExplored(const common::XY<size_t>& xy, size_t value)
	{
		GetExplored()[XYToRoomKey(xy)] = value;
	}

	static size_t GetExplored(const common::XY<size_t>& xy)
	{
		auto roomKey = XYToRoomKey(xy);
		if (GetExplored().count(roomKey) > 0)
		{
			return GetExplored()[roomKey];
		}
		return 0;
	}

	void SetExplored(const common::XY<size_t>& xy)
	{
		SetExplored(xy, GetExplored(xy) + 1);
	}

	bool IsExplored(const common::XY<size_t>& cell)
	{
		return GetExplored(cell) > 0;
	}

	common::XY<size_t> GetSize()
	{
		return common::XY<size_t>(COLUMNS, ROWS);
	}

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
		auto& borders = GetNSBorders();
		borders[IndexToString(index)] = (int)border;
	}

	static world::Border GetNSBorder(size_t index)
	{
		auto key = IndexToString(index);
		auto& borders = GetNSBorders();
		if (borders.count(key) > 0)
		{
			return (world::Border)(int)borders[key];
		}
		return world::Border::WALL;
	}

	static void SetEWBorder(size_t index, world::Border border)
	{
		auto& borders = GetEWBorders();
		borders[IndexToString(index)] = (int)border;
	}

	static world::Border GetEWBorder(size_t index)
	{
		auto key = IndexToString(index);
		auto& borders = GetEWBorders();
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
			return GetEWBorder(position.GetX() + position.GetY() * EW_BORDER_STRIDE);
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

	void Reset()
	{
		GetExplored().clear();
		auto worldSize = game::World::GetSize();
		maze::Maze maze(COLUMNS, ROWS);
		maze.Generate();

		for (auto column = 0; column < COLUMNS; ++column)
		{
			for (auto row = 0; row < ROWS; ++row)
			{
				size_t nsBorderIndex = column + row * NS_BORDER_STRIDE;
				size_t ewBorderIndex = column + row * EW_BORDER_STRIDE;

				auto cell = maze.GetCell((int)column, (int)row);
				auto northDoor = cell.value()->GetDoor(maze::Direction::NORTH);
				if (northDoor && *northDoor.value() == maze::Door::OPEN)
				{
					SetNSBorder(nsBorderIndex, world::Border::DOOR);
				}
				else
				{
					SetNSBorder(nsBorderIndex, world::Border::WALL);
				}
				auto westDoor = cell.value()->GetDoor(maze::Direction::WEST);
				if (westDoor && *westDoor.value() == maze::Door::OPEN)
				{
					SetEWBorder(ewBorderIndex, world::Border::DOOR);
				}
				else
				{
					SetEWBorder(ewBorderIndex, world::Border::WALL);
				}
			}
		}

	}
}
