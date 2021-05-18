#include "Game.World.Borders.h"
#include "Game.World.h"
#include <sstream>
#include "Game.Creatures.h"
#include "Game.World.Data.h"
#include <map>
#include <functional>
#include <tuple>
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

	world::Border GetNorthBorder(const common::XY<size_t>& position) 
	{ 
		return GetNSBorder(indexPlotters.find(IndexPlotter::NORTH)->second(position)); 
	}

	world::Border GetSouthBorder(const common::XY<size_t>& position)
	{
		return GetNSBorder(indexPlotters.find(IndexPlotter::SOUTH)->second(position));
	}

	world::Border GetEastBorder(const common::XY<size_t>& position)
	{
		return GetEWBorder(indexPlotters.find(IndexPlotter::EAST)->second(position));
	}

	world::Border GetWestBorder(const common::XY<size_t>& position)
	{
		return GetEWBorder(indexPlotters.find(IndexPlotter::WEST)->second(position));
	}

	typedef std::function<world::Border(const common::XY<size_t>&)> BorderGetter;

	const std::map<maze::Direction, std::tuple<BorderGetter, BorderGetter, BorderGetter>> borderGetters =
	{
		{maze::Direction::NORTH, {GetWestBorder,GetNorthBorder,GetEastBorder}},
		{maze::Direction::EAST, {GetNorthBorder,GetEastBorder,GetSouthBorder}},
		{maze::Direction::SOUTH, {GetEastBorder,GetSouthBorder,GetWestBorder}},
		{maze::Direction::WEST, {GetSouthBorder,GetWestBorder,GetNorthBorder}},
	};

	const int LEFT = 0;
	const int AHEAD = 1;
	const int RIGHT = 2;

	world::Border GetBorderAhead(const common::XY<size_t>& position, const maze::Direction& direction)
	{
		return std::get<AHEAD>(borderGetters.find(direction)->second)(position);
	}

	void SetNorthBorder(const common::XY<size_t>& position, world::Border border)
	{
		SetNSBorder(indexPlotters.find(IndexPlotter::NORTH)->second(position), border);
	}

	void SetSouthBorder(const common::XY<size_t>& position, world::Border border)
	{
		SetNSBorder(indexPlotters.find(IndexPlotter::SOUTH)->second(position), border);
	}

	void SetEastBorder(const common::XY<size_t>& position, world::Border border)
	{
		SetEWBorder(indexPlotters.find(IndexPlotter::EAST)->second(position), border);
	}

	void SetWestBorder(const common::XY<size_t>& position, world::Border border)
	{
		SetEWBorder(indexPlotters.find(IndexPlotter::WEST)->second(position), border);
	}

	const std::map<maze::Direction, std::function<void(const common::XY<size_t>&, world::Border)>> borderSetters =
	{
		{maze::Direction::NORTH, SetNorthBorder},
		{maze::Direction::EAST, SetEastBorder},
		{maze::Direction::SOUTH, SetSouthBorder},
		{maze::Direction::WEST, SetWestBorder},
	};

	void SetBorderAhead(const common::XY<size_t>& position, const maze::Direction& direction, const world::Border& border)
	{
		borderSetters.find(direction)->second(position, border);
	}

	world::Border GetBorderLeft(const common::XY<size_t>& position, const maze::Direction& direction)
	{
		return std::get<LEFT>(borderGetters.find(direction)->second)(position);
	}

	world::Border GetBorderRight(const common::XY<size_t>& position, const maze::Direction& direction)
	{
		return std::get<RIGHT>(borderGetters.find(direction)->second)(position);
	}

	static world::Border DetermineBorder(const std::shared_ptr<maze::Cell>& cell, const maze::Direction& direction)
	{
		auto mazeDoor = cell->GetDoor(direction);
		if (mazeDoor && *mazeDoor.value() == maze::Door::OPEN)
		{
			auto nextCell = cell->GetNeighbor(direction);
			if (cell->IsDeadEnd() || nextCell.value()->IsDeadEnd())
			{
				return world::Border::LOCK;
			}
			else
			{
				return world::Border::DOOR;
			}
		}
		else
		{
			return world::Border::WALL;
		}
	}

	void UpdateBorders(const maze::Maze& maze)
	{
		for (auto column = 0; column < maze.GetColumns(); ++column)
		{
			for (auto row = 0; row < maze.GetRows(); ++row)
			{
				common::XY<size_t> position = { (size_t)column, (size_t)row };
				auto cell = maze.GetCell((int)column, (int)row);

				SetNorthBorder(position, DetermineBorder(cell.value(), maze::Direction::NORTH));
				SetWestBorder(position, DetermineBorder(cell.value(), maze::Direction::WEST));
			}
		}
	}

	bool IsExitable(const common::XY<size_t>& position)
	{
		return
			GetNorthBorder(position) == game::world::Border::DOOR ||
			GetEastBorder(position) == game::world::Border::DOOR ||
			GetSouthBorder(position) == game::world::Border::DOOR ||
			GetWestBorder(position) == game::world::Border::DOOR;
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
