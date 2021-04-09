#include "Game.Avatar.h"
#include "Game.World.h"
#include "Game.Avatar.Statistics.h"
#include <vector>
#include "Common.Audio.h"
#include "Application.UIState.h"
#include "Application.Sounds.h"
#include "Common.RNG.h"
#include "Game.Creatures.h"
#include "Game.World.h"
namespace game::Avatar
{
	const size_t COLUMNS = 12;//TODO: i am duplicated!
	const size_t ROWS = 12;//TODO: i am duplicated!

	const int HUNGER_RATE = 1;

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
		if (!game::avatar::Statistics::IsDead())
		{
			avatarFacing =
				(avatarFacing == maze::Direction::NORTH) ? (maze::Direction::WEST) :
				(avatarFacing == maze::Direction::EAST) ? (maze::Direction::NORTH) :
				(avatarFacing == maze::Direction::SOUTH) ? (maze::Direction::EAST) :
				(maze::Direction::SOUTH);
		}
	}

	void TurnRight()
	{
		if (!game::avatar::Statistics::IsDead())
		{
			avatarFacing =
				(avatarFacing == maze::Direction::NORTH) ? (maze::Direction::EAST) :
				(avatarFacing == maze::Direction::EAST) ? (maze::Direction::SOUTH) :
				(avatarFacing == maze::Direction::SOUTH) ? (maze::Direction::WEST) :
				(maze::Direction::NORTH);
		}
	}

	void MoveAhead()
	{
		if (!game::avatar::Statistics::IsDead())
		{
			if (game::World::GetAhead() == game::world::Border::DOOR)
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
				game::World::SetExplored({avatarColumn, avatarRow});
				if (avatar::Statistics::IsStarving())
				{
					avatar::Statistics::Decrease(avatar::Statistic::HEALTH, HUNGER_RATE);
				}
				else
				{
					avatar::Statistics::Decrease(avatar::Statistic::HUNGER, HUNGER_RATE);
				}
			}
			else
			{
				common::audio::PlaySound(application::Sounds::BUMP_WALL);
			}
		}
		application::UIState::EnterGame();
	}

	void MoveBack()
	{
		TurnRight();
		TurnRight();
		MoveAhead();
		TurnRight();
		TurnRight();
	}

	void Reset()
	{
		::game::Avatar::avatarFacing = (maze::Direction)common::RNG::FromRange(0, (int)maze::Directions::All().size());
		do
		{
			::game::Avatar::avatarColumn = (size_t)common::RNG::FromRange(0, COLUMNS);
			::game::Avatar::avatarRow = (size_t)common::RNG::FromRange(0, ROWS);
		} while (game::Creatures::Read({ avatarColumn, avatarRow }));
		game::World::SetExplored({avatarColumn, avatarRow});
	}
}
