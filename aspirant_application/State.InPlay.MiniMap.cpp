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
#include "Application.OnEnter.h"
#include "Visuals.Layouts.h"
#include "Common.Audio.h"
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

	static void UpdateArrowImages(const std::set<std::string>& areas)
	{
		for (auto& areaImage : areaImages)
		{
			visuals::Images::SetVisible(LAYOUT_NAME, std::get<1>(areaImage), areas.contains(std::get<0>(areaImage)));
		}
	}
	static void UpdateMiniMapToolTip(const std::set<std::string>& areas, const common::XY<Sint32>& xy)
	{
		std::stringstream ss;
		if (areas.contains(AREA_WORLD_MAP))
		{
			auto area = visuals::Areas::Get(LAYOUT_NAME, AREA_WORLD_MAP);
			auto worldSize = game::World::GetSize();
			size_t cellWidth = area.size.GetX() / worldSize.GetX();
			size_t cellHeight = area.size.GetY() / worldSize.GetY();
			size_t cellColumn = ((size_t)xy.GetX() - area.xy.GetX()) / cellWidth;
			size_t cellRow = ((size_t)xy.GetY() - area.xy.GetY()) / cellHeight;
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

	static void OnMouseMotion(const common::XY<Sint32>& xy)
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		UpdateArrowImages(areas);
		UpdateMiniMapToolTip(areas, xy);
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

	static void OnEnter()
	{
		visuals::Layouts::InitializeLayout(LAYOUT_NAME);
		UpdateKeys();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_MAP, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_MAP, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_MAP, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::IN_PLAY_MAP, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_MAP, LAYOUT_NAME);
	}
}