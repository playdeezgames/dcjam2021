#include "Game.World.h"
#include <vector>
#include "Game.Avatar.h"
#include "Maze.h"
namespace game::World
{
	const size_t COLUMNS = 16;
	const size_t ROWS = 16;
	const size_t NS_BORDER_COUNT = ROWS * COLUMNS + COLUMNS;
	const size_t NS_BORDER_STRIDE = COLUMNS;
	const size_t EW_BORDER_COUNT = ROWS * COLUMNS + ROWS;
	const size_t EW_BORDER_STRIDE = COLUMNS + 1;

	common::XY<size_t> GetSize()
	{
		return common::XY<size_t>(COLUMNS, ROWS);
	}

	static std::vector<Border> nsBorders;
	static std::vector<Border> ewBorders;

	Border GetBorder(const common::XY<size_t>& position, const maze::Direction& direction)
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

	Border GetLeftSide()
	{
		auto position = game::Avatar::GetPosition();
		switch (game::Avatar::GetFacing())
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

	Border GetAhead()
	{
		auto position = game::Avatar::GetPosition();
		switch (game::Avatar::GetFacing())
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

	Border GetRightSide()
	{
		auto position = game::Avatar::GetPosition();
		switch (game::Avatar::GetFacing())
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
		nsBorders.reserve(NS_BORDER_COUNT);
		while (nsBorders.size() < NS_BORDER_COUNT)
		{
			nsBorders.push_back(Border::WALL);
		}

		ewBorders.reserve(EW_BORDER_COUNT);
		while (ewBorders.size() < EW_BORDER_COUNT)
		{
			ewBorders.push_back(Border::WALL);
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
					nsBorders[nsBorderIndex] = Border::DOOR;
				}
				auto westDoor = cell->GetDoor(maze::Direction::WEST);
				if (westDoor && *westDoor.value() == maze::Door::OPEN)
				{
					ewBorders[ewBorderIndex] = Border::DOOR;
				}
			}
		}
	}

	void Start()
	{
		Reset();
	}
}
