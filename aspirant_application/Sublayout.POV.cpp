#include <string>
#include "Game.Avatar.h"
#include <map>
#include "Game.World.h"
#include "Graphics.Texts.h"
#include "Graphics.Images.h"
#include "Game.Items.h"
#include "Game.World.Items.h"
#include "Application.Update.h"
#include "Game.Creatures.h"
namespace sublayout::POV
{
	const std::string LEFT_SIDE_IMAGE_ID = "LeftSide";
	const std::string AHEAD_IMAGE_ID = "Ahead";
	const std::string RIGHT_SIDE_IMAGE_ID = "RightSide";
	const std::string DIRECTION_TEXT_ID = "Direction";
	const std::string POSITION_TEXT_ID = "Position";
	const std::string FOOD_IMAGE_ID = "Food";
	const std::string POTION_IMAGE_ID = "Potion";
	const std::string BEER_IMAGE_ID = "Beer";
	const std::string WINE_IMAGE_ID = "Wine";
	const std::string COFFEE_IMAGE_ID = "Coffee";
	const std::string JOOLS_IMAGE_ID = "Jools";
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

	static void UpdateDirection()
	{
		auto facing = game::Avatar::GetFacing();
		::graphics::Texts::SetText(POV_LAYOUT_NAME, DIRECTION_TEXT_ID, directionNames[facing]);
	}

	const std::vector<game::Creature> creatures =
	{
		game::Creature::IMP,
		game::Creature::TROGLODYTE,
		game::Creature::BRAIN_TENTACLE,
		game::Creature::BULLY_MINION,
		game::Creature::EVIL_BAT,
		game::Creature::EVIL_MINION,
		game::Creature::WEREWOLF,
		game::Creature::WYVERN
	};

	static void UpdateCreatures()
	{
		for (auto creature : creatures)
		{
			auto imageId = game::creature::GetDescriptor(creature).imageId;
			auto instance = game::Creatures::GetInstance(game::Avatar::GetPosition());
			bool visible = instance && instance.value().creature == creature;
			graphics::Images::SetVisible(POV_LAYOUT_NAME, imageId, visible);
		}
	}

	static void UpdatePOV(const Uint32&)
	{
		auto position = game::Avatar::GetPosition();
		auto facing = game::Avatar::GetFacing();
		::graphics::Images::SetSprite(POV_LAYOUT_NAME, LEFT_SIDE_IMAGE_ID, leftSides[game::World::GetBorderLeft(position, facing)]);
		::graphics::Images::SetSprite(POV_LAYOUT_NAME, AHEAD_IMAGE_ID, aheads[game::World::GetBorderAhead(position, facing)]);
		::graphics::Images::SetSprite(POV_LAYOUT_NAME, RIGHT_SIDE_IMAGE_ID, rightSides[game::World::GetBorderRight(position, facing)]);
		::graphics::Images::SetVisible(POV_LAYOUT_NAME, FOOD_IMAGE_ID, game::world::Items::IsPresent(game::Item::FOOD, position));
		::graphics::Images::SetVisible(POV_LAYOUT_NAME, POTION_IMAGE_ID, game::world::Items::IsPresent(game::Item::POTION, position));
		::graphics::Images::SetVisible(POV_LAYOUT_NAME, BEER_IMAGE_ID, game::world::Items::IsPresent(game::Item::BEER, position));
		::graphics::Images::SetVisible(POV_LAYOUT_NAME, WINE_IMAGE_ID, game::world::Items::IsPresent(game::Item::WINE, position));
		::graphics::Images::SetVisible(POV_LAYOUT_NAME, COFFEE_IMAGE_ID, game::world::Items::IsPresent(game::Item::COFFEE, position));
		::graphics::Images::SetVisible(POV_LAYOUT_NAME, JOOLS_IMAGE_ID, game::world::Items::IsPresent(game::Item::JOOLS, position));
		UpdateDirection();
		UpdateCreatures();
	}

	void Start()
	{
		::application::Update::AddHandler(::UIState::IN_PLAY_MAP, UpdatePOV);
		::application::Update::AddHandler(::UIState::IN_PLAY_FLOOR, UpdatePOV);
		::application::Update::AddHandler(::UIState::IN_PLAY_INVENTORY, UpdatePOV);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdatePOV);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT, UpdatePOV);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, UpdatePOV);
	}
}