#include "json.hpp"
#include <SDL.h>
#include "Game.World.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Sprites.h"
#include <map>
#include "Game.Avatar.h"
#include "Game.Creatures.h"
#include "Visuals.Colors.h"
#include "Visuals.Data.Colors.h"
#include "Game.World.Borders.h"
#include "Game.Shoppes.h"
#include "Game.World.Items.h"
namespace visuals::WorldMap
{
	const std::string MAP_CELL_BASE = "MapCellBase";
	const std::string MAP_CELL_NORTH_WALL = "MapCellNorthWall";
	const std::string MAP_CELL_EAST_WALL = "MapCellEastWall";
	const std::string MAP_CELL_SOUTH_WALL = "MapCellSouthWall";
	const std::string MAP_CELL_WEST_WALL = "MapCellWestWall";
	const std::string MAP_CELL_NORTH_LOCKED = "MapCellNorthLocked";
	const std::string MAP_CELL_EAST_LOCKED = "MapCellEastLocked";
	const std::string MAP_CELL_SOUTH_LOCKED = "MapCellSouthLocked";
	const std::string MAP_CELL_WEST_LOCKED = "MapCellWestLocked";
	const std::string MAP_CELL_KNOWN = "MapCellKnown";
	const std::string MAP_CELL_KNOWN_NORTH_WALL = "MapCellKnownNorthWall";
	const std::string MAP_CELL_KNOWN_EAST_WALL = "MapCellKnownEastWall";
	const std::string MAP_CELL_KNOWN_SOUTH_WALL = "MapCellKnownSouthWall";
	const std::string MAP_CELL_KNOWN_WEST_WALL = "MapCellKnownWestWall";
	const std::string MAP_CELL_KNOWN_NORTH_LOCKED = "MapCellKnownNorthLocked";
	const std::string MAP_CELL_KNOWN_EAST_LOCKED = "MapCellKnownEastLocked";
	const std::string MAP_CELL_KNOWN_SOUTH_LOCKED = "MapCellKnownSouthLocked";
	const std::string MAP_CELL_KNOWN_WEST_LOCKED = "MapCellKnownWestLocked";
	const std::string MAP_CELL_UNEXPLORED = "MapCellUnexplored";
	const std::string AVATAR_NORTH = "AvatarNorth";
	const std::string AVATAR_EAST = "AvatarEast";
	const std::string AVATAR_SOUTH = "AvatarSouth";
	const std::string AVATAR_WEST = "AvatarWest";
	const std::string DANGER = "MapCellDanger";
	const std::string TRADE = "MapCellTrade";
	const std::string ITEM = "MapCellItem";

	static std::map<maze::Direction, std::string> wallSprites =
	{
		{maze::Direction::NORTH, MAP_CELL_NORTH_WALL},
		{maze::Direction::EAST, MAP_CELL_EAST_WALL},
		{maze::Direction::SOUTH, MAP_CELL_SOUTH_WALL},
		{maze::Direction::WEST, MAP_CELL_WEST_WALL}
	};

	static std::map<maze::Direction, std::string> lockedSprites =
	{
		{maze::Direction::NORTH, MAP_CELL_NORTH_LOCKED},
		{maze::Direction::EAST, MAP_CELL_EAST_LOCKED},
		{maze::Direction::SOUTH, MAP_CELL_SOUTH_LOCKED},
		{maze::Direction::WEST, MAP_CELL_WEST_LOCKED}
	};

	static std::map<maze::Direction, std::string> knownWallSprites =
	{
		{maze::Direction::NORTH, MAP_CELL_KNOWN_NORTH_WALL},
		{maze::Direction::EAST, MAP_CELL_KNOWN_EAST_WALL},
		{maze::Direction::SOUTH, MAP_CELL_KNOWN_SOUTH_WALL},
		{maze::Direction::WEST, MAP_CELL_KNOWN_WEST_WALL}
	};

	static std::map<maze::Direction, std::string> knownLockedSprites =
	{
		{maze::Direction::NORTH, MAP_CELL_KNOWN_NORTH_LOCKED},
		{maze::Direction::EAST, MAP_CELL_KNOWN_EAST_LOCKED},
		{maze::Direction::SOUTH, MAP_CELL_KNOWN_SOUTH_LOCKED},
		{maze::Direction::WEST, MAP_CELL_KNOWN_WEST_LOCKED}
	};

	static std::map<maze::Direction, std::string> avatarSprites =
	{
		{maze::Direction::NORTH, AVATAR_NORTH},
		{maze::Direction::EAST, AVATAR_EAST},
		{maze::Direction::SOUTH, AVATAR_SOUTH},
		{maze::Direction::WEST, AVATAR_WEST}
	};

	static void DrawWall(const std::shared_ptr<SDL_Renderer>& renderer, const common::XY<int>& plot, const common::XY<size_t>& cell, const maze::Direction& direction)
	{
		if (game::world::Borders::GetBorderAhead(cell, direction) == game::world::Border::WALL)
		{
			visuals::Sprites::Draw(wallSprites[direction], renderer, plot, visuals::Colors::Read(visuals::data::Colors::DEFAULT));
		}
	}

	static void DrawLocked(const std::shared_ptr<SDL_Renderer>& renderer, const common::XY<int>& plot, const common::XY<size_t>& cell, const maze::Direction& direction)
	{
		if (game::world::Borders::GetBorderAhead(cell, direction) == game::world::Border::LOCK)
		{
			visuals::Sprites::Draw(lockedSprites[direction], renderer, plot, visuals::Colors::Read(visuals::data::Colors::DEFAULT));
		}
	}

	static void DrawKnownWall(const std::shared_ptr<SDL_Renderer>& renderer, const common::XY<int>& plot, const common::XY<size_t>& cell, const maze::Direction& direction)
	{
		if (game::world::Borders::GetBorderAhead(cell, direction) == game::world::Border::WALL)
		{
			visuals::Sprites::Draw(knownWallSprites[direction], renderer, plot, visuals::Colors::Read(visuals::data::Colors::DEFAULT));
		}
	}

	static void DrawKnownLocked(const std::shared_ptr<SDL_Renderer>& renderer, const common::XY<int>& plot, const common::XY<size_t>& cell, const maze::Direction& direction)
	{
		if (game::world::Borders::GetBorderAhead(cell, direction) == game::world::Border::LOCK)
		{
			visuals::Sprites::Draw(knownLockedSprites[direction], renderer, plot, visuals::Colors::Read(visuals::data::Colors::DEFAULT));
		}
	}

	struct InternalWorldMap
	{
		common::XY<int> xy;
		common::XY<size_t> cellSize;
	};

	static std::vector<InternalWorldMap> internalWorldMaps;

	static void DrawItem(const std::shared_ptr<SDL_Renderer>& renderer, const common::XY<int>& plot, const common::XY<size_t>& cell)
	{
		if (game::world::Items::AnyPresent(cell))
		{
			visuals::Sprites::Draw(ITEM, renderer, plot, visuals::Colors::Read(visuals::data::Colors::DEFAULT));
		}
	}

	static void DrawDanger(const std::shared_ptr<SDL_Renderer>& renderer, const common::XY<int>& plot, const common::XY<size_t>& cell)
	{
		if (game::Creatures::GetInstance(cell))
		{
			visuals::Sprites::Draw(DANGER, renderer, plot, visuals::Colors::Read(visuals::data::Colors::DEFAULT));
		}
	}

	static void DrawTrade(const std::shared_ptr<SDL_Renderer>& renderer, const common::XY<int>& plot, const common::XY<size_t>& cell)
	{
		if (game::Shoppes::Read(cell))
		{
			visuals::Sprites::Draw(TRADE, renderer, plot, visuals::Colors::Read(visuals::data::Colors::DEFAULT));
		}
	}

	typedef std::function<void(const std::shared_ptr<SDL_Renderer>&, const common::XY<int>&, const common::XY<size_t>&, const maze::Direction&)> CellDirectionDrawer;

	static void DrawDirections(const std::shared_ptr<SDL_Renderer>& renderer, const common::XY<int>& plot, const common::XY<size_t>& cell, const std::vector<CellDirectionDrawer>& cellDirectionDrawers)
	{
		for (auto direction : maze::Directions::All())
		{
			for (auto& cellDrawer : cellDirectionDrawers)
			{
				cellDrawer(renderer, plot, cell, direction);
			}
		}
	}

	static void DrawAvatar(const std::shared_ptr<SDL_Renderer>& renderer, const common::XY<int>& plot, const common::XY<size_t>& cell)
	{
		if (cell == game::Avatar::GetPosition())
		{
			visuals::Sprites::Draw(avatarSprites[game::Avatar::GetFacing()], renderer, plot, visuals::Colors::Read(visuals::data::Colors::DEFAULT));
		}
	}

	static void DrawVisited(const std::shared_ptr<SDL_Renderer>& renderer, const common::XY<int>& plot, const common::XY<size_t>& cell)
	{
		visuals::Sprites::Draw(MAP_CELL_BASE, renderer, plot, visuals::Colors::Read(visuals::data::Colors::DEFAULT));
		DrawDirections(renderer, plot, cell, { DrawWall , DrawLocked });
		DrawItem(renderer, plot, cell);
		DrawDanger(renderer, plot, cell);
		DrawTrade(renderer, plot, cell);
		DrawAvatar(renderer, plot, cell);
	}

	static void DrawKnown(const std::shared_ptr<SDL_Renderer>& renderer, const common::XY<int>& plot, const common::XY<size_t>& cell)
	{
		visuals::Sprites::Draw(MAP_CELL_KNOWN, renderer, plot, visuals::Colors::Read(visuals::data::Colors::DEFAULT));
		DrawDirections(renderer, plot, cell, { DrawKnownWall , DrawKnownLocked });
		DrawDanger(renderer, plot, cell);
		DrawTrade(renderer, plot, cell);
	}

	static void DrawUnknown(const std::shared_ptr<SDL_Renderer>& renderer, const common::XY<int>& plot, const common::XY<size_t>&)
	{
		visuals::Sprites::Draw(MAP_CELL_UNEXPLORED, renderer, plot, visuals::Colors::Read(visuals::data::Colors::DEFAULT));
	}

	typedef std::function<void(const std::shared_ptr<SDL_Renderer>&, const common::XY<int>&, const common::XY<size_t>&)> CellDrawer;

	static std::map<game::world::KnownState, CellDrawer> knownStateDrawers =
	{
		{game::world::KnownState::VISITED, DrawVisited},
		{game::world::KnownState::KNOWN, DrawKnown},
		{game::world::KnownState::UNKNOWN, DrawUnknown}
	};

	static void DrawInternalWorldMap(const std::shared_ptr<SDL_Renderer>& renderer, size_t index)
	{
		auto& worldMap = internalWorldMaps[index];
		int x = worldMap.xy.GetX();
		int y = worldMap.xy.GetY();
		size_t cellWidth = worldMap.cellSize.GetX();
		size_t cellHeight = worldMap.cellSize.GetY();
		auto avatarPosition = game::Avatar::GetPosition();

		auto worldSize = game::World::GetSize();
		for (size_t column = 0u; column < worldSize.GetX(); ++column)
		{
			for (size_t row = 0u; row < worldSize.GetY(); ++row)
			{
				auto cell = common::XY<size_t>(column, row);
				auto plot = common::XY<int>((int)column * (int)cellWidth + x, (int)row * (int)cellHeight + y);
				knownStateDrawers[game::World::GetKnownState(cell)](renderer, plot, cell);
			}
		}
	}

	std::function<void(const std::shared_ptr<SDL_Renderer>&)> Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		size_t index = internalWorldMaps.size();
		internalWorldMaps.push_back(
			{
				common::XY<int>(
					model[common::data::Properties::X], 
					model[common::data::Properties::Y]),
				common::XY<size_t>(
					model[visuals::data::Properties::CELL_WIDTH], 
					model[visuals::data::Properties::CELL_HEIGHT])
			});
		return [index](const std::shared_ptr<SDL_Renderer>& renderer)
		{
			DrawInternalWorldMap(renderer, index);
		};
	}
}
