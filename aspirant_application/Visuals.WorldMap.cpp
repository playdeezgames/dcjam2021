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
namespace visuals::WorldMap
{
	const std::string MAP_CELL_BASE = "MapCellBase";
	const std::string MAP_CELL_NORTH_WALL = "MapCellNorthWall";
	const std::string MAP_CELL_EAST_WALL = "MapCellEastWall";
	const std::string MAP_CELL_SOUTH_WALL = "MapCellSouthWall";
	const std::string MAP_CELL_WEST_WALL = "MapCellWestWall";
	const std::string MAP_CELL_KNOWN = "MapCellKnown";
	const std::string MAP_CELL_KNOWN_NORTH_WALL = "MapCellKnownNorthWall";
	const std::string MAP_CELL_KNOWN_EAST_WALL = "MapCellKnownEastWall";
	const std::string MAP_CELL_KNOWN_SOUTH_WALL = "MapCellKnownSouthWall";
	const std::string MAP_CELL_KNOWN_WEST_WALL = "MapCellKnownWestWall";
	const std::string MAP_CELL_UNEXPLORED = "MapCellUnexplored";
	const std::string AVATAR_NORTH = "AvatarNorth";
	const std::string AVATAR_EAST = "AvatarEast";
	const std::string AVATAR_SOUTH = "AvatarSouth";
	const std::string AVATAR_WEST = "AvatarWest";
	const std::string DANGER = "MapCellDanger";

	static std::map<maze::Direction, std::string> wallSprites =
	{
		{maze::Direction::NORTH, MAP_CELL_NORTH_WALL},
		{maze::Direction::EAST, MAP_CELL_EAST_WALL},
		{maze::Direction::SOUTH, MAP_CELL_SOUTH_WALL},
		{maze::Direction::WEST, MAP_CELL_WEST_WALL}
	};

	static std::map<maze::Direction, std::string> knownWallSprites =
	{
		{maze::Direction::NORTH, MAP_CELL_KNOWN_NORTH_WALL},
		{maze::Direction::EAST, MAP_CELL_KNOWN_EAST_WALL},
		{maze::Direction::SOUTH, MAP_CELL_KNOWN_SOUTH_WALL},
		{maze::Direction::WEST, MAP_CELL_KNOWN_WEST_WALL}
	};

	static std::map<maze::Direction, std::string> avatarSprites =
	{
		{maze::Direction::NORTH, AVATAR_NORTH},
		{maze::Direction::EAST, AVATAR_EAST},
		{maze::Direction::SOUTH, AVATAR_SOUTH},
		{maze::Direction::WEST, AVATAR_WEST}
	};

	static void DrawWall(std::shared_ptr<SDL_Renderer> renderer, const common::XY<size_t>& cell, const common::XY<int>& plot, const maze::Direction& direction)
	{
		if (game::World::GetBorderAhead(cell, direction) == game::world::Border::WALL)
		{
			visuals::Sprites::Draw(wallSprites[direction], renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
		}
	}

	static void DrawKnownWall(std::shared_ptr<SDL_Renderer> renderer, const common::XY<size_t>& cell, const common::XY<int>& plot, const maze::Direction& direction)
	{
		if (game::World::GetBorderAhead(cell, direction) == game::world::Border::WALL)
		{
			visuals::Sprites::Draw(knownWallSprites[direction], renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
		}
	}

	struct InternalWorldMap
	{
		common::XY<int> xy;
		common::XY<size_t> cellSize;
	};

	static std::vector<InternalWorldMap> internalWorldMaps;

	static void DrawInternalWorldMap(std::shared_ptr<SDL_Renderer> renderer, size_t index)
	{
		auto& worldMap = internalWorldMaps[index];
		int x = worldMap.xy.GetX();
		int y = worldMap.xy.GetY();
		size_t cellWidth = worldMap.cellSize.GetX();
		size_t cellHeight = worldMap.cellSize.GetY();
		auto avatarPosition = game::Avatar::GetPosition();

		auto worldSize = game::World::GetSize();
		for (size_t column = 0; column < worldSize.GetX(); ++column)
		{
			for (size_t row = 0; row < worldSize.GetY(); ++row)
			{
				auto cell = common::XY<size_t>(column, row);
				auto plot = common::XY<int>((int)column * cellWidth + x, (int)row * cellHeight + y);
				switch (game::World::GetKnownState(cell))
				{
				case game::world::KnownState::VISITED:
				{

					visuals::Sprites::Draw(MAP_CELL_BASE, renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
					DrawWall(renderer, cell, plot, maze::Direction::NORTH);
					DrawWall(renderer, cell, plot, maze::Direction::EAST);
					DrawWall(renderer, cell, plot, maze::Direction::SOUTH);
					DrawWall(renderer, cell, plot, maze::Direction::WEST);

					if (cell == avatarPosition)
					{
						visuals::Sprites::Draw(avatarSprites[game::Avatar::GetFacing()], renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
					}

					if (game::Creatures::GetInstance(cell))
					{
						visuals::Sprites::Draw(DANGER, renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
					}
				}
				break;
				case game::world::KnownState::KNOWN:
				{

					visuals::Sprites::Draw(MAP_CELL_KNOWN, renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
					DrawKnownWall(renderer, cell, plot, maze::Direction::NORTH);
					DrawKnownWall(renderer, cell, plot, maze::Direction::EAST);
					DrawKnownWall(renderer, cell, plot, maze::Direction::SOUTH);
					DrawKnownWall(renderer, cell, plot, maze::Direction::WEST);

					if (cell == avatarPosition)
					{
						visuals::Sprites::Draw(avatarSprites[game::Avatar::GetFacing()], renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
					}

					if (game::Creatures::GetInstance(cell))
					{
						visuals::Sprites::Draw(DANGER, renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
					}
				}
				break;
				default:
				{
					visuals::Sprites::Draw(MAP_CELL_UNEXPLORED, renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
				}
				break;
				}
			}
		}
	}

	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string& layoutName, const nlohmann::json& model)
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
		return [index](std::shared_ptr<SDL_Renderer> renderer)
		{
			DrawInternalWorldMap(renderer, index);
		};
	}


	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		int x = model[common::data::Properties::X];
		int y = model[common::data::Properties::Y];
		int cellWidth = model[visuals::data::Properties::CELL_WIDTH];
		int cellHeight = model[visuals::data::Properties::CELL_WIDTH];
		auto avatarPosition = game::Avatar::GetPosition();

		auto worldSize = game::World::GetSize();
		for (size_t column = 0; column < worldSize.GetX(); ++column)
		{
			for (size_t row = 0; row < worldSize.GetY(); ++row)
			{
				auto cell = common::XY<size_t>(column, row);
				auto plot = common::XY<int>((int)column * cellWidth + x, (int)row * cellHeight + y);
				switch (game::World::GetKnownState(cell))
				{
				case game::world::KnownState::VISITED:
				{

					visuals::Sprites::Draw(MAP_CELL_BASE, renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
					DrawWall(renderer, cell, plot, maze::Direction::NORTH);
					DrawWall(renderer, cell, plot, maze::Direction::EAST);
					DrawWall(renderer, cell, plot, maze::Direction::SOUTH);
					DrawWall(renderer, cell, plot, maze::Direction::WEST);

					if (cell == avatarPosition)
					{
						visuals::Sprites::Draw(avatarSprites[game::Avatar::GetFacing()], renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
					}

					if (game::Creatures::GetInstance(cell))
					{
						visuals::Sprites::Draw(DANGER, renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
					}
				}
				break;
				case game::world::KnownState::KNOWN:
				{

					visuals::Sprites::Draw(MAP_CELL_KNOWN, renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
					DrawKnownWall(renderer, cell, plot, maze::Direction::NORTH);
					DrawKnownWall(renderer, cell, plot, maze::Direction::EAST);
					DrawKnownWall(renderer, cell, plot, maze::Direction::SOUTH);
					DrawKnownWall(renderer, cell, plot, maze::Direction::WEST);

					if (cell == avatarPosition)
					{
						visuals::Sprites::Draw(avatarSprites[game::Avatar::GetFacing()], renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
					}

					if (game::Creatures::GetInstance(cell))
					{
						visuals::Sprites::Draw(DANGER, renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
					}
				}
				break;
				default:
				{
					visuals::Sprites::Draw(MAP_CELL_UNEXPLORED, renderer, plot, visuals::Colors::Read("White"));//TODO: hard coded string
				}
				break;
				}
			}
		}
	}
}
