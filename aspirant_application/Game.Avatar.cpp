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

	static void ActuallyTurnLeft()
	{
		SetFacing(
			(GetFacing() == maze::Direction::NORTH) ? (maze::Direction::WEST) :
			(GetFacing() == maze::Direction::EAST) ? (maze::Direction::NORTH) :
			(GetFacing() == maze::Direction::SOUTH) ? (maze::Direction::EAST) :
			(maze::Direction::SOUTH));
	}

	static void ActuallyTurnRight()
	{
		SetFacing(
			(GetFacing() == maze::Direction::NORTH) ? (maze::Direction::EAST) :
			(GetFacing() == maze::Direction::EAST) ? (maze::Direction::SOUTH) :
			(GetFacing() == maze::Direction::SOUTH) ? (maze::Direction::WEST) :
			(maze::Direction::NORTH));
	}

	std::optional<std::string> TurnLeft()
	{
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HEALTH))
		{
			if (game::avatar::Statistics::IsMinimum(game::avatar::Statistic::NAUSEA))
			{
				ActuallyTurnLeft();
			}
			else
			{
				ActuallyTurnRight();
			}
		}
		return std::nullopt;
	}

	std::optional<std::string> TurnRight()
	{
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HEALTH))
		{
			if (game::avatar::Statistics::IsMinimum(game::avatar::Statistic::NAUSEA))
			{
				ActuallyTurnRight();
			}
			else
			{
				ActuallyTurnLeft();
			}
		}
		return std::nullopt;
	}

	std::optional<std::string> Poop()
	{
		int itemId = ::data::Stores::GetStore(::data::Store::AVATAR)[data::Properties::POOP];
		game::avatar::Statistics::Write(game::avatar::Statistic::BOWEL, game::avatar::Statistics::Default(game::avatar::Statistic::BOWEL));
		game::world::Items::Add(GetPosition(), itemId, 1);
		return application::Sounds::Read(application::UI::Sfx::HUNTER_POOPS);
	}

	static void HandleHunger()
	{
		int hungerRate = ::data::Stores::GetStore(::data::Store::AVATAR)[game::data::Properties::HUNGER_RATE];
		if (game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HUNGER))
		{
			avatar::Statistics::Decrease(avatar::Statistic::HEALTH, hungerRate);
		}
		else
		{
			avatar::Statistics::Decrease(avatar::Statistic::HUNGER, hungerRate);
		}
	}

	static std::optional<std::string> HandleBowel()
	{
		if (game::avatar::Statistics::IsMaximum(game::avatar::Statistic::BOWEL))//are you poopin'?
		{
			return Poop();
		}
		return std::nullopt;
	}

	static void HandleNausea()
	{
		int sobrietyRate = ::data::Stores::GetStore(::data::Store::AVATAR)[game::data::Properties::SOBRIETY_RATE];
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::NAUSEA))
		{
			avatar::Statistics::Decrease(avatar::Statistic::NAUSEA, sobrietyRate);
		}
	}

	static void HandleSobriety()
	{
		int sobrietyRate = ::data::Stores::GetStore(::data::Store::AVATAR)[game::data::Properties::SOBRIETY_RATE];
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::DRUNKENNESS))
		{
			avatar::Statistics::Decrease(avatar::Statistic::DRUNKENNESS, sobrietyRate);
		}
	}

	std::optional<std::string> MoveAhead()
	{
		std::optional<std::string> result = std::nullopt;
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HEALTH))
		{
			auto border = game::World::GetBorderAhead(game::Avatar::GetPosition(), game::Avatar::GetFacing());
			if (border == game::world::Border::DOOR)
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
				HandleHunger();
				HandleNausea();
				HandleSobriety();
				result = HandleBowel();
			}
			else if (border== game::world::Border::LOCK)
			{
				if (game::avatar::Statistics::Read(game::avatar::Statistic::KEYS)>0)
				{
					game::avatar::Statistics::Decrease(game::avatar::Statistic::KEYS, 1);
					game::World::SetBorderAhead(game::Avatar::GetPosition(), game::Avatar::GetFacing(), game::world::Border::DOOR);
					//TODO: play an unlock sound!
				}
				else
				{
					//TODO: failed to unlock sound!
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

	bool CanPoop()
	{
		int poopThreshold = ::data::Stores::GetStore(::data::Store::AVATAR)[game::data::Properties::POOP_THRESHOLD];
		return (game::avatar::Statistics::Read(game::avatar::Statistic::BOWEL) >= poopThreshold);
	}

	void Reset()
	{
		auto worldSize = game::World::GetSize();
		SetFacing((maze::Direction)common::RNG::FromRange(0, (int)maze::Directions::All().size()));
		do
		{
			SetColumn((size_t)common::RNG::FromRange(0, (int)worldSize.GetX()));
			SetRow((size_t)common::RNG::FromRange(0, (int)worldSize.GetY()));
		} while (!game::World::CanSpawnAvatar(GetPosition()));
		game::World::SetExplored(GetPosition());
	}
}
