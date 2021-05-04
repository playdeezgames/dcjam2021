#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Visuals.Images.h"
#include "Visuals.Texts.h"
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
#include "Visuals.Areas.h"
#include "Game.Creatures.h"
namespace state::in_play::MiniMap
{
	const std::string LAYOUT_NAME = "State.InPlay.MiniMap";
	const std::string AREA_TURN_LEFT = "TurnLeft";
	const std::string AREA_TURN_RIGHT = "TurnRight";
	const std::string AREA_MOVE_AHEAD = "MoveAhead";
	const std::string AREA_WORLD_MAP = "WorldMap";
	const std::string IMAGE_MOVE_AHEAD = "MoveAhead";
	const std::string IMAGE_TURN_RIGHT = "TurnRight";
	const std::string IMAGE_TURN_LEFT = "TurnLeft";
	const std::string TEXT_MAP_TOOL_TIP = "MapToolTip";

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
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		for (auto& areaImage : areaImages)
		{
			visuals::Images::SetVisible(LAYOUT_NAME, std::get<1>(areaImage), areas.contains(std::get<0>(areaImage)));
		}
		if (areas.contains(AREA_WORLD_MAP))
		{
			auto area = visuals::Areas::Get(LAYOUT_NAME, AREA_WORLD_MAP);
			auto worldSize = game::World::GetSize();
			size_t cellWidth = area.size.GetX() / worldSize.GetX();
			size_t cellHeight = area.size.GetY() / worldSize.GetY();
			common::XY<size_t> worldPosition = { (xy.GetX() - area.xy.GetX()) / cellWidth , (xy.GetY() - area.xy.GetY()) / cellHeight };
			if (game::World::IsExplored(worldPosition) == game::world::KnownState::UNKNOWN)
			{
				visuals::Texts::SetText(LAYOUT_NAME, TEXT_MAP_TOOL_TIP, "????");
			}
			else
			{
				auto creature = game::Creatures::GetInstance(worldPosition);
				if (creature)
				{
					visuals::Texts::SetText(LAYOUT_NAME, TEXT_MAP_TOOL_TIP, creature.value().descriptor.name);
				}
				else
				{
					visuals::Texts::SetText(LAYOUT_NAME, TEXT_MAP_TOOL_TIP, "(empty)");
				}
			}

		}
		else
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_MAP_TOOL_TIP, "");
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
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
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