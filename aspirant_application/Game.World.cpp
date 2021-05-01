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
namespace game::World
{
	std::string XYToRoomKey(const common::XY<size_t>& xy)
	{
		std::stringstream ss;
		ss << "(" << xy.GetX() << "," << xy.GetY() << ")";
		return ss.str();
	}

	static nlohmann::json& GetNSBorders()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::NS_BORDERS) == 0)
		{
			data[game::data::Properties::NS_BORDERS] = nlohmann::json();
		}
		return data[game::data::Properties::NS_BORDERS];
	}

	static nlohmann::json& GetEWBorders()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::EW_BORDERS) == 0)
		{
			data[game::data::Properties::EW_BORDERS] = nlohmann::json();
		}
		return data[game::data::Properties::EW_BORDERS];
	}

	static nlohmann::json& GetExplored()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::EXPLORED) == 0)
		{
			data[game::data::Properties::EXPLORED] = nlohmann::json();
		}
		return data[game::data::Properties::EXPLORED];
	}

	const size_t COLUMNS = 12;
	const size_t ROWS = 12;
	const size_t NS_BORDER_COUNT = ROWS * COLUMNS + COLUMNS;
	const size_t NS_BORDER_STRIDE = COLUMNS;
	const size_t EW_BORDER_COUNT = ROWS * COLUMNS + ROWS;
	const size_t EW_BORDER_STRIDE = COLUMNS + 1;

	static void SetExplored(const common::XY<size_t>& xy, size_t value)
	{
		GetExplored()[XYToRoomKey(xy)] = value;
	}

	static std::optional<size_t> GetExplored(const common::XY<size_t>& xy)
	{
		auto roomKey = XYToRoomKey(xy);
		if (GetExplored().count(roomKey) > 0)
		{
			return GetExplored()[roomKey];
		}
		return std::nullopt;
	}

	void SetExplored(const common::XY<size_t>& xy)
	{
		auto visits = GetExplored(xy);
		if (visits)
		{
			SetExplored(xy, visits.value() + 1);
		}
		else
		{
			SetExplored(xy, 1);
		}
	}

	void SetKnown(const common::XY<size_t>& xy)
	{
		auto visits = GetExplored(xy);
		if (!visits)
		{
			SetExplored(xy, 0);
		}
	}


	game::world::KnownState IsExplored(const common::XY<size_t>& cell)
	{
		auto visits = GetExplored(cell);
		if (visits)
		{
			return (visits.value() > 0) ? (game::world::KnownState::VISITED) : (game::world::KnownState::KNOWN);
		}
		return game::world::KnownState::UNKNOWN;
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

	static void PostGenerateMaze(maze::Maze& maze)
	{
		auto worldSize = game::World::GetSize();
		size_t extraDoors = ::data::Stores::GetStore(::data::Store::MAZE)[data::Properties::EXTRA_DOORS];
		while (extraDoors > 0)
		{
			size_t x = common::RNG::FromRange(0u, worldSize.GetX());
			size_t y = common::RNG::FromRange(0u, worldSize.GetY());
			auto cell = maze.GetCell((int)x, (int)y);
			if (cell)
			{
				maze::Direction direction = maze::Directions::All()[common::RNG::FromRange(0u, maze::Directions::All().size())];
				if ((*cell).get()->SetDoor(direction, maze::Door::OPEN))
				{
					extraDoors--;
				}
			}
		}
	}

	void Reset()
	{
		GetExplored().clear();
		auto worldSize = game::World::GetSize();
		maze::Maze maze(COLUMNS, ROWS);
		maze.Generate();
		PostGenerateMaze(maze);

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
