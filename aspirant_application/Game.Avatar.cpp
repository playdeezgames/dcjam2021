#include "Game.Avatar.h"
#include "Game.World.h"
namespace game::Avatar
{
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
		if (game::World::GetAhead() == game::World::Border::DOOR)
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
}
