#include "Game.Avatar.h"
#include "Game.World.h"
#include "Game.Avatar.Statistics.h"
#include <vector>
#include "Common.Sounds.h"
#include "Application.UIState.h"
#include "Application.Sounds.h"
namespace game::Avatar
{
	const int HUNGER_RATE = 1;

	size_t avatarColumn = 0;
	size_t avatarRow = 0;
	maze::Direction avatarFacing = maze::Direction::EAST;

	static std::vector<std::vector<size_t>> explored;

	static void SetExplored()
	{
		explored[avatarColumn][avatarRow]++;
	}

	bool IsExplored(const common::XY<size_t>& cell)
	{
		return explored[cell.GetX()][cell.GetY()] > 0;
	}


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
				SetExplored();
				//do hunger!
				if (avatar::Statistics::IsStarving())
				{
					avatar::Statistics::Decrease(avatar::Statistic::HEALTH, HUNGER_RATE);
				}
				else
				{
					avatar::Statistics::Decrease(avatar::Statistic::HUNGER, HUNGER_RATE);
				}
				application::UIState::EnterGame();
			}
			else
			{
				common::Sounds::PlaySound(application::Sounds::BUMP_WALL);
			}
		}
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
		SetExplored();
	}
}
