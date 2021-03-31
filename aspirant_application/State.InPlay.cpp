#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.Images.h"
#include <vector>
#include <map>
#include "Game.h"
namespace state::InPlay
{
	const std::string LAYOUT_NAME = "State.InPlay";
	const std::string LEFT_SIDE = "LeftSide";
	const std::string AHEAD = "Ahead";
	const std::string RIGHT_SIDE = "RightSide";

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
		case ::Command::RED:
			application::UIState::Write(::UIState::MAIN_MENU);
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
		::graphics::Images::SetSprite(LAYOUT_NAME, LEFT_SIDE, (game::GetLeftSide() == game::Border::DOOR) ? ("LeftDoor") : ("LeftWall"));
		::graphics::Images::SetSprite(LAYOUT_NAME, AHEAD, (game::GetAhead() == game::Border::DOOR) ? ("AheadDoor") : ("AheadWall"));
		::graphics::Images::SetSprite(LAYOUT_NAME, RIGHT_SIDE, (game::GetRightSide() == game::Border::DOOR) ? ("RightDoor") : ("RightWall"));
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY, OnUpdate);
	}
}