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
#include "Game.Data.Properties.h"
#include "Game.h"
#include "Data.Stores.h"
#include "Game.World.Items.h"
namespace game::Avatar
{
	const int HUNGER_RATE = 1;

	nlohmann::json& GetAvatar()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::AVATAR) == 0)
		{
			data[game::data::Properties::AVATAR] = nlohmann::json();
		}
		return data[game::data::Properties::AVATAR];
	}

	static void SetFacing(maze::Direction direction)
	{
		GetAvatar()[game::data::Properties::FACING] = (int)direction;
	}

	maze::Direction GetFacing()
	{
		return (maze::Direction)(int)GetAvatar()[game::data::Properties::FACING];
	}

	static void SetColumn(size_t column)
	{
		GetAvatar()[game::data::Properties::COLUMN] = column;
	}

	static void SetRow(size_t row)
	{
		GetAvatar()[game::data::Properties::ROW] = row;
	}

	void SetPosition(const common::XY<size_t>& position)
	{
		SetColumn(position.GetX());
		SetRow(position.GetY());
	}

	common::XY<size_t> GetPosition()
	{
		return { (size_t)GetAvatar()[game::data::Properties::COLUMN], (size_t)GetAvatar()[game::data::Properties::ROW] };
	}

	std::optional<std::string> TurnLeft()
	{
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HEALTH))
		{
			SetFacing(
				(GetFacing() == maze::Direction::NORTH) ? (maze::Direction::WEST) :
				(GetFacing() == maze::Direction::EAST) ? (maze::Direction::NORTH) :
				(GetFacing() == maze::Direction::SOUTH) ? (maze::Direction::EAST) :
				(maze::Direction::SOUTH));
		}
		return std::nullopt;
	}

	std::optional<std::string> TurnRight()
	{
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HEALTH))
		{
			SetFacing(
				(GetFacing() == maze::Direction::NORTH) ? (maze::Direction::EAST) :
				(GetFacing() == maze::Direction::EAST) ? (maze::Direction::SOUTH) :
				(GetFacing() == maze::Direction::SOUTH) ? (maze::Direction::WEST) :
				(maze::Direction::NORTH));
		}
		return std::nullopt;
	}

	static std::optional<std::string> Poop()
	{
		int itemId = ::data::Stores::GetStore(::data::Store::AVATAR)[data::Properties::POOP];
		game::avatar::Statistics::Write(game::avatar::Statistic::BOWEL, game::avatar::Statistics::Default(game::avatar::Statistic::BOWEL));
		game::world::Items::Add(GetPosition(), itemId, 1);
		return application::Sounds::Read(application::UI::Sfx::HUNTER_POOPS);
	}

	std::optional<std::string> MoveAhead()
	{
		std::optional<std::string> result = std::nullopt;
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HEALTH))
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
				if (game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HUNGER))
				{
					avatar::Statistics::Decrease(avatar::Statistic::HEALTH, HUNGER_RATE);
				}
				else
				{
					avatar::Statistics::Decrease(avatar::Statistic::HUNGER, HUNGER_RATE);
				}
				if (game::avatar::Statistics::IsMaximum(game::avatar::Statistic::BOWEL))//are you poopin'?
				{
					result = Poop();
				}
			}
			else
			{
				result = application::Sounds::Read(application::UI::Sfx::BUMP_WALL);
			}
		}
		common::audio::Sfx::Play(application::UIState::EnterGame());
		return result;
	}

	std::optional<std::string> MoveBack()
	{
		TurnRight();
		TurnRight();
		auto result = MoveAhead();
		TurnRight();
		TurnRight();
		return result;
	}

	void Reset()
	{
		auto worldSize = game::World::GetSize();
		SetFacing((maze::Direction)common::RNG::FromRange(0, (int)maze::Directions::All().size()));
		do
		{
			SetColumn((size_t)common::RNG::FromRange(0, (int)worldSize.GetX()));
			SetRow((size_t)common::RNG::FromRange(0, (int)worldSize.GetY()));
		} while (game::Creatures::GetInstance(GetPosition()));
		game::World::SetExplored(GetPosition());
	}
}
