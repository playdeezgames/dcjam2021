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
#include "Game.Item.h"
#include <sstream>
#include "States.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Graphics.Areas.h"
namespace state::in_play::MiniMap
{
	const std::string LAYOUT_NAME = "State.InPlay.MiniMap";

	static void LeavePlay()
	{
		application::UIState::Write(::UIState::LEAVE_PLAY);
	}

	static void GoToFloorInventory()
	{
		application::UIState::Write(::UIState::IN_PLAY_FLOOR);
	}

	static void GoToAvatarStatus()
	{
		application::UIState::Write(::UIState::IN_PLAY_STATUS);
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, LeavePlay },
		{ ::Command::LEFT, game::Avatar::TurnLeft },
		{ ::Command::RIGHT, game::Avatar::TurnRight },
		{ ::Command::UP, game::Avatar::MoveAhead },
		{ ::Command::DOWN, game::Avatar::MoveBack },
		{ ::Command::NEXT, GoToFloorInventory },
		{ ::Command::YELLOW, GoToFloorInventory },
		{ ::Command::PREVIOUS, GoToAvatarStatus }
	};

	static void OnCommand(const ::Command& command)
	{
		common::Utility::Dispatch(commandHandlers, command);
	}


	void Start()
	{
		::application::Command::SetHandlers(::UIState::IN_PLAY_MAP, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_MAP, LAYOUT_NAME);
	}
}