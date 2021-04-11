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
#include "json.hpp"
#include "Game.Properties.h"
namespace game
{
	extern nlohmann::json data;
}
namespace game::Avatar
{
	const int HUNGER_RATE = 1;

	static nlohmann::json& GetAvatar()
	{
		if (data.count(game::Properties::AVATAR) == 0)
		{
			data[game::Properties::AVATAR] = nlohmann::json();
		}
		return data[game::Properties::AVATAR];
	}

	static void SetFacing(maze::Direction direction)
	{
		GetAvatar()[game::Properties::FACING] = (int)direction;
	}

	maze::Direction GetFacing()
	{
		return (maze::Direction)(int)GetAvatar()[game::Properties::FACING];
	}

	static void SetColumn(size_t column)
	{
		GetAvatar()[game::Properties::COLUMN] = column;
	}

	static void SetRow(size_t row)
	{
		GetAvatar()[game::Properties::ROW] = row;
	}

	static void SetPosition(const common::XY<size_t> position)
	{
		SetColumn(position.GetX());
		SetRow(position.GetY());
	}

	common::XY<size_t> GetPosition()
	{
		return { (size_t)GetAvatar()[game::Properties::COLUMN], (size_t)GetAvatar()[game::Properties::ROW] };
	}

	void TurnLeft()
	{
		if (!game::avatar::Statistics::IsDead())
		{
			SetFacing(
				(GetFacing() == maze::Direction::NORTH) ? (maze::Direction::WEST) :
				(GetFacing() == maze::Direction::EAST) ? (maze::Direction::NORTH) :
				(GetFacing() == maze::Direction::SOUTH) ? (maze::Direction::EAST) :
				(maze::Direction::SOUTH));
		}
	}

	void TurnRight()
	{
		if (!game::avatar::Statistics::IsDead())
		{
			SetFacing(
				(GetFacing() == maze::Direction::NORTH) ? (maze::Direction::EAST) :
				(GetFacing() == maze::Direction::EAST) ? (maze::Direction::SOUTH) :
				(GetFacing() == maze::Direction::SOUTH) ? (maze::Direction::WEST) :
				(maze::Direction::NORTH));
		}
	}

	void MoveAhead()
	{
		if (!game::avatar::Statistics::IsDead())
		{
			if (game::World::GetBorderAhead(game::Avatar::GetPosition(), game::Avatar::GetFacing()) == game::world::Border::DOOR)
			{
				switch (GetFacing())
				{
				case maze::Direction::NORTH:
					SetRow(GetPosition().GetY() - 1);
					break;
				case maze::Direction::EAST:
					SetColumn(GetPosition().GetX() + 1);
					break;
				case maze::Direction::SOUTH:
					SetRow(GetPosition().GetY() + 1);
					break;
				case maze::Direction::WEST:
					SetColumn(GetPosition().GetX() - 1);
					break;
				}
				game::World::SetExplored(GetPosition());
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
		auto worldSize = game::World::GetSize();
		SetFacing((maze::Direction)common::RNG::FromRange(0, (int)maze::Directions::All().size()));
		do
		{
			SetColumn((size_t)common::RNG::FromRange(0, (int)worldSize.GetX()));
			SetRow((size_t)common::RNG::FromRange(0, (int)worldSize.GetY()));
		} while (game::Creatures::Read(GetPosition()));
		game::World::SetExplored(GetPosition());
	}
}
