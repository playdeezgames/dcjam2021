#include "json.hpp"
#include <SDL.h>
#include "Game.World.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
#include "Graphics.Sprites.h"
#include <map>
#include "Game.Avatar.h"
namespace graphics::WorldMap 
{ 
	const std::string MAP_CELL_BASE = "MapCellBase";
	const std::string MAP_CELL_NORTH_WALL = "MapCellNorthWall";
	const std::string MAP_CELL_EAST_WALL = "MapCellEastWall";
	const std::string MAP_CELL_SOUTH_WALL = "MapCellSouthWall";
	const std::string MAP_CELL_WEST_WALL = "MapCellWestWall";
	const std::string MAP_CELL_UNEXPLORED = "MapCellUnexplored";
	const std::string AVATAR_NORTH = "AvatarNorth";
	const std::string AVATAR_EAST = "AvatarEast";
	const std::string AVATAR_SOUTH = "AvatarSouth";
	const std::string AVATAR_WEST = "AvatarWest";

	static std::map<maze::Direction, std::string> wallSprites =
	{
		{maze::Direction::NORTH, MAP_CELL_NORTH_WALL},
		{maze::Direction::EAST, MAP_CELL_EAST_WALL},
		{maze::Direction::SOUTH, MAP_CELL_SOUTH_WALL},
		{maze::Direction::WEST, MAP_CELL_WEST_WALL}
	};

	static std::map<maze::Direction, std::string> avatarSprites =
	{
		{maze::Direction::NORTH, AVATAR_NORTH},
		{maze::Direction::EAST, AVATAR_EAST},
		{maze::Direction::SOUTH, AVATAR_SOUTH},
		{maze::Direction::WEST, AVATAR_WEST}
	};

	static void DrawWall(SDL_Renderer* renderer, const common::XY<size_t>& cell, const common::XY<int>& plot, const maze::Direction& direction)
	{
		if (game::World::GetBorder(cell, direction) == game::World::Border::WALL)
		{
			graphics::Sprites::Read(wallSprites[direction]).value().Draw(renderer, plot);
		}
	}

	void Draw(SDL_Renderer* renderer, const nlohmann::json& model)
	{
		int x = model[common::Properties::X];
		int y = model[common::Properties::Y];
		int cellWidth = model[graphics::Properties::CELL_WIDTH];
		int cellHeight = model[graphics::Properties::CELL_WIDTH];
		auto avatarPosition = game::Avatar::GetPosition();

		auto worldSize = game::World::GetSize();
		for (size_t column = 0; column < worldSize.GetX(); ++column)
		{
			for (size_t row = 0; row < worldSize.GetY(); ++row)
			{
				auto cell = common::XY<size_t>(column, row);
				auto plot = common::XY<int>((int)column * cellWidth + x, (int)row * cellHeight + y);
				if (game::Avatar::IsExplored(cell))
				{

					graphics::Sprites::Read(MAP_CELL_BASE).value().Draw(renderer, plot);
					DrawWall(renderer, cell, plot, maze::Direction::NORTH);
					DrawWall(renderer, cell, plot, maze::Direction::EAST);
					DrawWall(renderer, cell, plot, maze::Direction::SOUTH);
					DrawWall(renderer, cell, plot, maze::Direction::WEST);

					if (cell == avatarPosition)
					{
						graphics::Sprites::Read(avatarSprites[game::Avatar::GetFacing()]).value().Draw(renderer, plot);
					}
				}
				else
				{
					graphics::Sprites::Read(MAP_CELL_UNEXPLORED).value().Draw(renderer, plot);
				}
			}
		}
	}
}