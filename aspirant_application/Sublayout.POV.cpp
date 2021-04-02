#include <string>
#include "Game.Avatar.h"
#include <map>
#include "Game.World.h"
#include "Graphics.Texts.h"
#include "Graphics.Images.h"
#include "Game.Items.h"
#include "Application.Update.h"
namespace sublayout::POV
{
	const std::string LEFT_SIDE_IMAGE_ID = "LeftSide";
	const std::string AHEAD_IMAGE_ID = "Ahead";
	const std::string RIGHT_SIDE_IMAGE_ID = "RightSide";
	const std::string DIRECTION_TEXT_ID = "Direction";
	const std::string POSITION_TEXT_ID = "Position";
	const std::string FOOD_IMAGE_ID = "Food";
	const std::string POTION_IMAGE_ID = "Potion";
	const std::string POV_LAYOUT_NAME = "Sublayout.POV";

	static std::map<game::World::Border, std::string> leftSides =
	{
		{game::World::Border::DOOR, "LeftDoor"},
		{game::World::Border::WALL, "LeftWall"}
	};

	static std::map<game::World::Border, std::string> rightSides =
	{
		{game::World::Border::DOOR, "RightDoor"},
		{game::World::Border::WALL, "RightWall"}
	};

	static std::map<game::World::Border, std::string> aheads =
	{
		{game::World::Border::DOOR, "AheadDoor"},
		{game::World::Border::WALL, "AheadWall"}
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

	static void UpdatePOV(const Uint32&)
	{
		::graphics::Images::SetSprite(POV_LAYOUT_NAME, LEFT_SIDE_IMAGE_ID, leftSides[game::World::GetLeftSide()]);
		::graphics::Images::SetSprite(POV_LAYOUT_NAME, AHEAD_IMAGE_ID, aheads[game::World::GetAhead()]);
		::graphics::Images::SetSprite(POV_LAYOUT_NAME, RIGHT_SIDE_IMAGE_ID, rightSides[game::World::GetRightSide()]);
		auto position = game::Avatar::GetPosition();
		::graphics::Images::SetVisible(POV_LAYOUT_NAME, FOOD_IMAGE_ID, game::Items::IsPresent(game::Item::FOOD, position));
		::graphics::Images::SetVisible(POV_LAYOUT_NAME, POTION_IMAGE_ID, game::Items::IsPresent(game::Item::POTION, position));
		UpdateDirection();
	}

	void Start()
	{
		::application::Update::AddHandler(::UIState::IN_PLAY_MAP, UpdatePOV);
		::application::Update::AddHandler(::UIState::IN_PLAY_FLOOR, UpdatePOV);
	}
}