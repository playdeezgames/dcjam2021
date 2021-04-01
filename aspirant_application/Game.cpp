#include "Game.h"
#include "Maze.h"
namespace game
{
	const size_t COLUMNS = 6;
	const size_t ROWS = 8;
	const size_t NS_BORDER_COUNT = ROWS * COLUMNS + COLUMNS;
	const size_t NS_BORDER_STRIDE = COLUMNS;
	const size_t EW_BORDER_COUNT = ROWS * COLUMNS + ROWS;
	const size_t EW_BORDER_STRIDE = COLUMNS + 1;

	static std::vector<Border> nsBorders;//(rows + 1) * (columns)
	static std::vector<Border> ewBorders;//(columns + 1) * (rows)

	size_t avatarColumn = 0;
	size_t avatarRow = 0;
	maze::Direction avatarFacing = maze::Direction::EAST;
	maze::Direction GetFacing()
	{
		return avatarFacing;
	}
	common::XY<size_t> GetPosition()
	{
		return { avatarColumn, avatarRow };
	}


	Border GetLeftSide()
	{
		switch (avatarFacing)
		{
		case maze::Direction::NORTH:
			return ewBorders[avatarColumn + avatarRow * EW_BORDER_STRIDE];
		case maze::Direction::EAST:
			return nsBorders[avatarColumn + avatarRow * NS_BORDER_STRIDE];
		case maze::Direction::SOUTH:
			return ewBorders[avatarColumn + avatarRow * EW_BORDER_STRIDE + 1];
		default:
			return nsBorders[avatarColumn + avatarRow * NS_BORDER_STRIDE + NS_BORDER_STRIDE];
		}
	}

	Border GetAhead()
	{
		switch (avatarFacing)
		{
		case maze::Direction::NORTH:
			return nsBorders[avatarColumn + avatarRow * NS_BORDER_STRIDE];
		case maze::Direction::EAST:
			return ewBorders[avatarColumn + avatarRow * EW_BORDER_STRIDE + 1];
		case maze::Direction::SOUTH:
			return nsBorders[avatarColumn + avatarRow * NS_BORDER_STRIDE + NS_BORDER_STRIDE];
		default:
			return ewBorders[avatarColumn + avatarRow * EW_BORDER_STRIDE];
		}
	}

	Border GetRightSide()
	{
		switch (avatarFacing)
		{
		case maze::Direction::NORTH:
			return ewBorders[avatarColumn + avatarRow * EW_BORDER_STRIDE + 1];
		case maze::Direction::EAST:
			return nsBorders[avatarColumn + avatarRow * NS_BORDER_STRIDE + NS_BORDER_STRIDE];
		case maze::Direction::SOUTH:
			return ewBorders[avatarColumn + avatarRow * EW_BORDER_STRIDE];
		default:
			return nsBorders[avatarColumn + avatarRow * NS_BORDER_STRIDE];
		}
	}

	void TurnLeft()
	{
		avatarFacing =
			(avatarFacing == maze::Direction::NORTH) ? (maze::Direction::WEST) :
			(avatarFacing == maze::Direction::EAST) ? (maze::Direction::NORTH) :
			(avatarFacing == maze::Direction::SOUTH) ? (maze::Direction::EAST) :
			(maze::Direction::SOUTH);
	}

	void TurnRight()
	{
		avatarFacing =
			(avatarFacing == maze::Direction::NORTH) ? (maze::Direction::EAST) :
			(avatarFacing == maze::Direction::EAST) ? (maze::Direction::SOUTH) :
			(avatarFacing == maze::Direction::SOUTH) ? (maze::Direction::WEST) :
			(maze::Direction::NORTH);
	}

	void MoveAhead()
	{
		if (GetAhead() == Border::DOOR)
		{
			switch (avatarFacing)
			{
			case maze::Direction::NORTH:
				avatarRow--;
				break;
			case maze::Direction::EAST:
				avatarColumn++;
				break;
			case maze::Direction::SOUTH:
				avatarRow++;
				break;
			case maze::Direction::WEST:
				avatarColumn--;
				break;
			}
		}
	}


	void Start()
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
}