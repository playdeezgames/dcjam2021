#include "Game.World.h"
#include <vector>
#include "Maze.h"
#include "Common.RNG.h"
#include "Maze.Direction.h"
#include "Game.h"
#include "Game.Data.Properties.h"
#include <sstream>
#include "Data.Stores.h"
#include "Game.Creatures.h"
#include <optional>
#include "Game.World.Borders.h"
#include "Game.World.Data.h"

#include "Game.Shoppes.h"
namespace game::World
{
	static void SetExplored(const common::XY<size_t>& xy, size_t value)
	{
		game::world::Data::GetExplored()[game::world::Data::XYToRoomKey(xy)] = value;
	}

	static std::optional<size_t> GetExplored(const common::XY<size_t>& xy)
	{
		auto roomKey = game::world::Data::XYToRoomKey(xy);
		if (game::world::Data::GetExplored().count(roomKey) > 0)
		{
			return game::world::Data::GetExplored()[roomKey];
		}
		return std::nullopt;
	}

	void SetExplored(const common::XY<size_t>& xy)
	{
		auto visits = GetExplored(xy);
		if (visits)
		{
			SetExplored(xy, visits.value() + 1);
		}
		else
		{
			SetExplored(xy, 1);
		}
	}

	void SetKnown(const common::XY<size_t>& xy)
	{
		auto visits = GetExplored(xy);
		if (!visits)
		{
			SetExplored(xy, 0);
		}
	}


	game::world::KnownState GetKnownState(const common::XY<size_t>& cell)
	{
		auto visits = GetExplored(cell);
		if (visits)
		{
			return (visits.value() > 0) ? (game::world::KnownState::VISITED) : (game::world::KnownState::KNOWN);
		}
		return game::world::KnownState::UNKNOWN;
	}

	common::XY<size_t> GetSize()
	{
		return common::XY<size_t>(COLUMNS, ROWS);
	}

	static size_t GetExtraDoorCount(const game::Difficulty& difficulty)
	{
		return ::data::Stores::GetStore(::data::Store::MAZE)[data::Properties::EXTRA_DOORS][(int)difficulty];
	}

	static void PostGenerateMaze(maze::Maze& maze, const game::Difficulty& difficulty)
	{
		auto worldSize = game::World::GetSize();
		size_t extraDoors = GetExtraDoorCount(difficulty);
		while (extraDoors > 0)
		{
			size_t x = common::RNG::FromRange(0u, worldSize.GetX());
			size_t y = common::RNG::FromRange(0u, worldSize.GetY());
			auto cell = maze.GetCell((int)x, (int)y);
			if (cell)
			{
				maze::Direction direction = maze::Directions::All()[common::RNG::FromRange(0u, maze::Directions::All().size())];
				if ((*cell).get()->SetDoor(direction, maze::Door::OPEN))
				{
					extraDoors--;
				}
			}
		}
	}

	void Reset(const game::Difficulty& difficulty)
	{
		game::world::Data::ClearExplored();
		auto worldSize = game::World::GetSize();
		maze::Maze maze(worldSize.GetX(), worldSize.GetY(), maze::Directions::All(), maze::Directions::Opposite);
		maze.Generate();
		PostGenerateMaze(maze, difficulty);
		game::world::Borders::UpdateBorders(maze);
	}

}
