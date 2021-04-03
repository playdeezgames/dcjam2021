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
#include "Game.Avatar.Statistics.h"
#include "Game.Items.h"
#include <sstream>
namespace state::in_play::MiniMap
{
	const std::string LAYOUT_NAME = "State.InPlay.MiniMap";

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
			application::UIState::Write(::UIState::LEAVE_PLAY);
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
		case ::Command::DOWN:
			game::Avatar::MoveBack();
			break;
		case ::Command::NEXT:
		case ::Command::YELLOW:
			application::UIState::Write(::UIState::IN_PLAY_FLOOR);
			break;
		case ::Command::PREVIOUS:
			application::UIState::Write(::UIState::IN_PLAY_STATUS);
			break;
		}
	}



	static void OnUpdate(const Uint32& ticks)
	{
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_MAP, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_MAP, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_MAP, OnUpdate);
	}
}