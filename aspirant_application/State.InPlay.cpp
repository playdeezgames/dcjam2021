#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.Images.h"
#include "Graphics.Texts.h"
#include <vector>
#include <map>
#include "Game.Avatar.h"
#include "Game.World.h"
#include <sstream>
namespace state::InPlay
{
	const std::string LAYOUT_NAME = "State.InPlay";
	const std::string LEFT_SIDE_IMAGE_ID = "LeftSide";
	const std::string AHEAD_IMAGE_ID = "Ahead";
	const std::string RIGHT_SIDE_IMAGE_ID = "RightSide";
	const std::string DIRECTION_TEXT_ID = "Direction";
	const std::string POSITION_TEXT_ID = "Position";
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
		{maze::Direction::NORTH, "North"},
		{maze::Direction::EAST,  "East"},
		{maze::Direction::SOUTH, "South"},
		{maze::Direction::WEST, "West"}
	};

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
		case ::Command::RED:
			application::UIState::Write(::UIState::MAIN_MENU);
			break;
		case ::Command::LEFT:
			game::Avatar::TurnLeft();
			break;
		case ::Command::RIGHT:
			game::Avatar::TurnRight();
			break;
		case ::Command::UP:
			game::Avatar::MoveAhead();
			break;
		}
	}


	static void OnUpdate(const Uint32& ticks)
	{
		auto facing = game::Avatar::GetFacing();

		::graphics::Images::SetSprite(LAYOUT_NAME, LEFT_SIDE_IMAGE_ID, leftSides[game::World::GetLeftSide()]);
		::graphics::Images::SetSprite(LAYOUT_NAME, AHEAD_IMAGE_ID, aheads[game::World::GetAhead()]);
		::graphics::Images::SetSprite(LAYOUT_NAME, RIGHT_SIDE_IMAGE_ID, rightSides[game::World::GetRightSide()]);
		::graphics::Texts::SetTextText(LAYOUT_NAME, DIRECTION_TEXT_ID, directionNames[facing]);

		auto position = game::Avatar::GetPosition();
		std::stringstream ss;
		ss << "(" << position.GetX() << ", " << position.GetY() << ")";
		::graphics::Texts::SetTextText(LAYOUT_NAME, POSITION_TEXT_ID, ss.str());
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY, OnUpdate);
	}
}