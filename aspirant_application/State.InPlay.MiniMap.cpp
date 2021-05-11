#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Visuals.Images.h"
#include "Visuals.Texts.h"
#include "Game.Avatar.h"
#include "Game.World.h"
#include "Game.Avatar.Statistics.h"
#include <sstream>
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Game.Creatures.h"
#include "Application.OnEnter.h"
#include "Common.Audio.h"
#include "Visuals.Areas.h"
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
	const std::string CELL_UNKNOWN = "????";
	const std::string CELL_EMPTY = "(empty)";
	const std::string TEXT_KEYS = "Keys";
	const std::string EMPTY_TOOL_TIP = "";
	const std::string NO_ARROW = "";

	static void MoveAhead()
	{
		common::audio::Sfx::Play(game::Avatar::MoveAhead());
		::application::OnEnter::Handle();
	}

	static void MoveBack()
	{
		common::audio::Sfx::Play(game::Avatar::MoveBack());
		::application::OnEnter::Handle();
	}

	static void TurnLeft()
	{
		common::audio::Sfx::Play(game::Avatar::TurnLeft());
		::application::OnEnter::Handle();
	}

	static void TurnRight()
	{
		common::audio::Sfx::Play(game::Avatar::TurnRight());
		::application::OnEnter::Handle();
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{ ::Command::LEFT, TurnLeft },
		{ ::Command::RIGHT, TurnRight },
		{ ::Command::UP, MoveAhead },
		{ ::Command::DOWN, MoveBack },
		{ ::Command::NEXT, application::UIState::GoTo(::UIState::IN_PLAY_FLOOR) },
		{ ::Command::YELLOW, application::UIState::GoTo(::UIState::IN_PLAY_FLOOR) },
		{ ::Command::PREVIOUS, application::UIState::GoTo(::UIState::IN_PLAY_STATUS) }
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

	static void UpdateArrowImages(const std::string& area)
	{
		for (auto& areaImage : areaImages)
		{
			visuals::Images::SetVisible(LAYOUT_NAME, std::get<1>(areaImage), area == std::get<0>(areaImage));
		}
	}
	static void UpdateMiniMapToolTip(const std::string& area, const common::XY<Sint32>& xy)
	{
		std::stringstream ss;
		if (area == AREA_WORLD_MAP)
		{
			auto area = visuals::Areas::Get(LAYOUT_NAME, AREA_WORLD_MAP);
			auto worldSize = game::World::GetSize();
			size_t cellWidth = area.size.GetX() / worldSize.GetX();
			size_t cellHeight = area.size.GetY() / worldSize.GetY();
			size_t cellColumn = ((size_t)xy.GetX()) / cellWidth;
			size_t cellRow = ((size_t)xy.GetY()) / cellHeight;
			common::XY<size_t> worldPosition = { cellColumn , cellRow };
			if (game::World::GetKnownState(worldPosition) == game::world::KnownState::UNKNOWN)
			{
				ss << CELL_UNKNOWN;
			}
			else
			{
				auto creature = game::Creatures::GetInstance(worldPosition);
				ss << ((creature.has_value()) ? (creature.value().descriptor.name) : (CELL_EMPTY));
			}
		}
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_MAP_TOOL_TIP, ss.str());
	}

	static void OnMouseMotionInArea(const std::string& area, const common::XY<Sint32>& xy)
	{
		UpdateArrowImages(area);
		UpdateMiniMapToolTip(area, xy);
	}
	static void OnMouseMotionOutsideAreas(const common::XY<Sint32>&)
	{
		UpdateArrowImages(NO_ARROW);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_MAP_TOOL_TIP, EMPTY_TOOL_TIP);
	}

	const std::map<std::string, std::function<bool()>> mouseUpHandlers =
	{
		{ AREA_MOVE_AHEAD, []() {game::Avatar::MoveAhead(); application::OnEnter::Handle(); return true; }},
		{ AREA_TURN_LEFT, []() {game::Avatar::TurnLeft(); return true; }},
		{ AREA_TURN_RIGHT, []() {game::Avatar::TurnRight(); return true; }}
	};

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		return common::Utility::Dispatch(mouseUpHandlers, area, false);
	}

	static void UpdateKeys()
	{
		std::stringstream ss;
		auto keyCount = game::avatar::Statistics::Read(game::avatar::Statistic::KEYS);
		if (keyCount > 0)
		{
			ss << "Keys: ";
			ss << keyCount;
		}
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_KEYS, ss.str());
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_MAP, UpdateKeys);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_MAP, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_MAP, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideAreas));
		::application::Command::SetHandlers(::UIState::IN_PLAY_MAP, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_MAP, LAYOUT_NAME);
	}
}