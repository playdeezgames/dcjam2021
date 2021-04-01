#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.Images.h"
#include "Graphics.Texts.h"
#include <vector>
#include <map>
#include "Game.h"
#include <sstream>
namespace state::InPlay
{
	const std::string LAYOUT_NAME = "State.InPlay";
	const std::string LEFT_SIDE_IMAGE_ID = "LeftSide";
	const std::string AHEAD_IMAGE_ID = "Ahead";
	const std::string RIGHT_SIDE_IMAGE_ID = "RightSide";
	const std::string DIRECTION_TEXT_ID = "Direction";
	const std::string POSITION_TEXT_ID = "Position";

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
		case ::Command::RED:
			application::UIState::Write(::UIState::MAIN_MENU);
			break;
		case ::Command::LEFT:
			game::TurnLeft();
			break;
		case ::Command::RIGHT:
			game::TurnRight();
			break;
		case ::Command::UP:
			game::MoveAhead();
			break;
		}
	}

	static void OnUpdate(const Uint32& ticks)
	{
		auto facing = game::GetFacing();

		::graphics::Images::SetSprite(LAYOUT_NAME, LEFT_SIDE_IMAGE_ID, (game::GetLeftSide() == game::Border::DOOR) ? ("LeftDoor") : ("LeftWall"));
		::graphics::Images::SetSprite(LAYOUT_NAME, AHEAD_IMAGE_ID, (game::GetAhead() == game::Border::DOOR) ? ("AheadDoor") : ("AheadWall"));
		::graphics::Images::SetSprite(LAYOUT_NAME, RIGHT_SIDE_IMAGE_ID, (game::GetRightSide() == game::Border::DOOR) ? ("RightDoor") : ("RightWall"));
		::graphics::Texts::SetTextText(LAYOUT_NAME, DIRECTION_TEXT_ID,
			(facing == maze::Direction::NORTH) ? "North" :
			(facing == maze::Direction::EAST) ? "East" :
			(facing == maze::Direction::SOUTH) ? "South" :
			"West");

		auto position = game::GetPosition();
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