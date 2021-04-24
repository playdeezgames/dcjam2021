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
	const std::string AREA_TURN_LEFT = "TurnLeft";
	const std::string AREA_TURN_RIGHT = "TurnRight";
	const std::string AREA_MOVE_AHEAD = "MoveAhead";
	const std::string IMAGE_MOVE_AHEAD = "MoveAhead";
	const std::string IMAGE_TURN_RIGHT = "TurnRight";
	const std::string IMAGE_TURN_LEFT = "TurnLeft";

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

	const std::vector<std::tuple<std::string, std::string>> areaImages =
	{
		{AREA_MOVE_AHEAD, IMAGE_MOVE_AHEAD},
		{AREA_TURN_LEFT, IMAGE_TURN_LEFT},
		{AREA_TURN_RIGHT, IMAGE_TURN_RIGHT}
	};

	static void OnMouseMotion(const common::XY<Sint32>& xy)
	{
		auto areas = graphics::Areas::Get(LAYOUT_NAME, xy);
		for (auto& areaImage : areaImages)
		{
			graphics::Images::SetVisible(LAYOUT_NAME, std::get<1>(areaImage), areas.contains(std::get<0>(areaImage)));
		}
	}

	const std::map<std::string, std::function<bool()>> mouseUpHandlers =
	{
		{ AREA_MOVE_AHEAD, []() {game::Avatar::MoveAhead(); return true; }},
		{ AREA_TURN_LEFT, []() {game::Avatar::TurnLeft(); return true; }},
		{ AREA_TURN_RIGHT, []() {game::Avatar::TurnRight(); return true; }}
	};

	static bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8)
	{
		auto areas = graphics::Areas::Get(LAYOUT_NAME, xy);
		bool result = false;
		for (auto& area : areas)
		{
			result = result || common::Utility::Dispatch(mouseUpHandlers, area, false);
		}
		return result;
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_MAP, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_MAP, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::IN_PLAY_MAP, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_MAP, LAYOUT_NAME);
	}
}