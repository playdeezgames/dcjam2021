#include <string>
#include "Game.Avatar.h"
#include <map>
#include "Game.World.h"
#include "Graphics.Texts.h"
#include "Graphics.Images.h"
#include "Game.Item.h"
#include "Game.World.Items.h"
#include "Application.Update.h"
#include "Game.Creatures.h"
#include <tuple>
namespace sublayout::POV
{
	const std::string LEFT_SIDE_IMAGE_ID = "LeftSide";
	const std::string AHEAD_IMAGE_ID = "Ahead";
	const std::string RIGHT_SIDE_IMAGE_ID = "RightSide";
	const std::string DIRECTION_TEXT_ID = "Direction";
	const std::string POSITION_TEXT_ID = "Position";
	const std::string POV_LAYOUT_NAME = "Sublayout.POV";

	static std::map<game::world::Border, std::string> leftSides =
	{
		{game::world::Border::DOOR, "LeftDoor"},
		{game::world::Border::WALL, "LeftWall"}
	};

	static std::map<game::world::Border, std::string> rightSides =
	{
		{game::world::Border::DOOR, "RightDoor"},
		{game::world::Border::WALL, "RightWall"}
	};

	static std::map<game::world::Border, std::string> aheads =
	{
		{game::world::Border::DOOR, "AheadDoor"},
		{game::world::Border::WALL, "AheadWall"}
	};

	static std::map<maze::Direction, std::string> directionNames =
	{
		{maze::Direction::NORTH, "N"},
		{maze::Direction::EAST,  "E"},
		{maze::Direction::SOUTH, "S"},
		{maze::Direction::WEST, "W"}
	};

	static void UpdateDirection(maze::Direction facing	)
	{
		::graphics::Texts::SetText(POV_LAYOUT_NAME, DIRECTION_TEXT_ID, directionNames[facing]);
	}

	static void UpdateCreatures(const common::XY<size_t> position)
	{
		auto creatures = game::creature::All();
		for (auto creature : creatures)
		{
			auto imageId = game::creature::GetDescriptor(creature).imageId;
			auto instance = game::Creatures::GetInstance(position);
			bool visible = instance && instance.value().creature == creature;
			graphics::Images::SetVisible(POV_LAYOUT_NAME, imageId, visible);
		}
	}

	static void UpdateRoom(const common::XY<size_t> position, const maze::Direction& facing)
	{
		::graphics::Images::SetSprite(POV_LAYOUT_NAME, LEFT_SIDE_IMAGE_ID, leftSides[game::World::GetBorderLeft(position, facing)]);
		::graphics::Images::SetSprite(POV_LAYOUT_NAME, AHEAD_IMAGE_ID, aheads[game::World::GetBorderAhead(position, facing)]);
		::graphics::Images::SetSprite(POV_LAYOUT_NAME, RIGHT_SIDE_IMAGE_ID, rightSides[game::World::GetBorderRight(position, facing)]);
	}

	const std::vector<game::Item> items =
	{
		game::Item::FOOD,
		game::Item::POTION,
		game::Item::BEER,
		game::Item::WINE,
		game::Item::COFFEE,
		game::Item::JOOLS
	};

	static void UpdateItems(const common::XY<size_t> position)
	{
		for (auto& item : items)
		{
			::graphics::Images::SetVisible(POV_LAYOUT_NAME, game::item::GetDescriptor(item).imageId, game::world::Items::IsPresent(item, position));
		}
	}

	static void UpdatePOV(const Uint32&)
	{
		auto position = game::Avatar::GetPosition();
		auto facing = game::Avatar::GetFacing();
		UpdateRoom(position, facing);
		UpdateItems(position);
		UpdateDirection(facing);
		UpdateCreatures(position);
	}

	const std::vector<::UIState> states = 
	{
		::UIState::IN_PLAY_MAP,
		::UIState::IN_PLAY_FLOOR,
		::UIState::IN_PLAY_INVENTORY,
		::UIState::IN_PLAY_STATUS,
		::UIState::IN_PLAY_COMBAT,
		::UIState::IN_PLAY_COMBAT_RESULT
	};

	void Start()
	{
		for (auto state : states)
		{
			::application::Update::AddHandler(state, UpdatePOV);
		}
	}
}