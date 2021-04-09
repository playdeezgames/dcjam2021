#include "Game.World.h"
#include <vector>
#include "Maze.h"
#include "Common.RNG.h"
#include "Maze.Direction.h"
namespace game::World
{
	const size_t COLUMNS = 12;
	const size_t ROWS = 12;
	const size_t NS_BORDER_COUNT = ROWS * COLUMNS + COLUMNS;
	const size_t NS_BORDER_STRIDE = COLUMNS;
	const size_t EW_BORDER_COUNT = ROWS * COLUMNS + ROWS;
	const size_t EW_BORDER_STRIDE = COLUMNS + 1;

	static std::vector<std::vector<size_t>> explored;

	void SetExplored(const common::XY<size_t>& xy)
	{
		explored[xy.GetX()][xy.GetY()]++;
	}

	bool IsExplored(const common::XY<size_t>& cell)
	{
		return explored[cell.GetX()][cell.GetY()] > 0;
	}

	common::XY<size_t> GetSize()
	{
		return common::XY<size_t>(COLUMNS, ROWS);
	}

	static std::vector<world::Border> nsBorders;
	static std::vector<world::Border> ewBorders;

	world::Border GetBorderAhead(const common::XY<size_t>& position, const maze::Direction& direction)
	{
		switch (direction)
		{
		case maze::Direction::NORTH:
			return nsBorders[position.GetX() + position.GetY() * NS_BORDER_STRIDE];
		case maze::Direction::EAST:
			return ewBorders[position.GetX() + position.GetY() * EW_BORDER_STRIDE + 1];
		case maze::Direction::SOUTH:
			return nsBorders[position.GetX() + position.GetY() * NS_BORDER_STRIDE + NS_BORDER_STRIDE];
		default:
			return ewBorders[position.GetX() + position.GetY() * EW_BORDER_STRIDE];
		}
	}

	world::Border GetBorderLeft(const common::XY<size_t>& position, const maze::Direction& direction)
	{
		switch (direction)
		{
		case maze::Direction::NORTH:
			return ewBorders[position.GetX() + position.GetY() * EW_BORDER_STRIDE];
		case maze::Direction::EAST:
			return nsBorders[position.GetX() + position.GetY() * NS_BORDER_STRIDE];
		case maze::Direction::SOUTH:
			return ewBorders[position.GetX() + position.GetY() * EW_BORDER_STRIDE + 1];
		default:
			return nsBorders[position.GetX() + position.GetY() * NS_BORDER_STRIDE + NS_BORDER_STRIDE];
		}
	}

	world::Border GetBorderRight(const common::XY<size_t>& position, const maze::Direction& direction)
	{
		switch (direction)
		{
		case maze::Direction::NORTH:
			return ewBorders[position.GetX() + position.GetY() * EW_BORDER_STRIDE + 1];
		case maze::Direction::EAST:
			return nsBorders[position.GetX() + position.GetY() * NS_BORDER_STRIDE + NS_BORDER_STRIDE];
		case maze::Direction::SOUTH:
			return ewBorders[position.GetX() + position.GetY() * EW_BORDER_STRIDE];
		default:
			return nsBorders[position.GetX() + position.GetY() * NS_BORDER_STRIDE];
		}
	}

	void Reset()
	{
		explored.clear();
		auto worldSize = game::World::GetSize();
		while (explored.size() < worldSize.GetX())
		{
			explored.push_back(std::vector<size_t>());
			auto& column = explored.back();
			while (column.size() < worldSize.GetY())
			{
				column.push_back(0);
			}
		}
		nsBorders.reserve(NS_BORDER_COUNT);
		nsBorders.clear();
		while (nsBorders.size() < NS_BORDER_COUNT)
		{
			nsBorders.push_back(world::Border::WALL);
		}

		ewBorders.reserve(EW_BORDER_COUNT);
		ewBorders.clear();
		while (ewBorders.size() < EW_BORDER_COUNT)
		{
			ewBorders.push_back(world::Border::WALL);
		}

		maze::Maze maze(COLUMNS, ROWS);
		maze.Generate();

		for (auto column = 0; column < COLUMNS; ++column)
		{
			for (auto row = 0; row < ROWS; ++row)
			{
				size_t nsBorderIndex = column + row * NS_BORDER_STRIDE;
				size_t ewBorderIndex = column + row * EW_BORDER_STRIDE;

				auto cell = maze.GetCell((int)column, (int)row);
				auto northDoor = cell->GetDoor(maze::Direction::NORTH);
				if (northDoor && *northDoor.value() == maze::Door::OPEN)
				{
					nsBorders[nsBorderIndex] = world::Border::DOOR;
				}
				auto westDoor = cell->GetDoor(maze::Direction::WEST);
				if (westDoor && *westDoor.value() == maze::Door::OPEN)
				{
					ewBorders[ewBorderIndex] = world::Border::DOOR;
				}
			}
		}

	}
}
