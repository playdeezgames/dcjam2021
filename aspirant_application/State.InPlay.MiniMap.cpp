#include <sstream>
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Creatures.h"
#include "Game.Shoppes.h"
#include "Game.World.h"
#include "Visuals.Areas.h"
#include "Visuals.Images.h"
#include "Visuals.Texts.h"
namespace state::in_play::MiniMap
{
	const std::string LAYOUT_NAME = "State.InPlay.MiniMap";
	const std::string AREA_TURN_LEFT = "TurnLeft";
	const std::string AREA_TURN_RIGHT = "TurnRight";
	const std::string AREA_MOVE_AHEAD = "MoveAhead";
	const std::string AREA_WORLD_MAP = "WorldMap";
	const std::string AREA_TRADE = "Trade";
	const std::string IMAGE_MOVE_AHEAD = "MoveAhead";
	const std::string IMAGE_TURN_RIGHT = "TurnRight";
	const std::string IMAGE_TURN_LEFT = "TurnLeft";
	const std::string IMAGE_TRADE = "Trade";
	const std::string TEXT_MAP_TOOL_TIP = "MapToolTip";
	const std::string CELL_UNKNOWN = "????";
	const std::string CELL_EMPTY = "(empty)";
	const std::string TEXT_KEYS = "Keys";
	const std::string EMPTY_TOOL_TIP = "";
	const std::string NO_ARROW = "";

	static std::function<void()> DoSomethingAndRefresh(std::function<std::optional<std::string>()> action)
	{
		return [action]() {
			common::audio::Sfx::Play(action());
			application::OnEnter::Handle();
		};
	}

	static bool HasShoppe()
	{
		return game::Shoppes::Read(game::Avatar::GetPosition()).has_value();
	}

	static void EnterTrade()
	{
		if (HasShoppe())
		{
			application::UIState::Write(::UIState::IN_PLAY_TRADE);
		}
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{ ::Command::LEFT, DoSomethingAndRefresh(game::Avatar::TurnLeft) },
		{ ::Command::RIGHT, DoSomethingAndRefresh(game::Avatar::TurnRight) },
		{ ::Command::UP, DoSomethingAndRefresh(game::Avatar::MoveAhead) },
		{ ::Command::DOWN, DoSomethingAndRefresh(game::Avatar::MoveBack) },
		{ ::Command::NEXT, application::UIState::GoTo(::UIState::IN_PLAY_FLOOR) },
		{ ::Command::BLUE, EnterTrade },
		{ ::Command::YELLOW, application::UIState::GoTo(::UIState::IN_PLAY_FLOOR) },
		{ ::Command::PREVIOUS, application::UIState::GoTo(::UIState::IN_PLAY_STATUS) }
	};

	static void OnCommand(const ::Command& command)
	{
		common::Utility::Dispatch(commandHandlers, command);
	}

	static bool AlwaysShow()
	{
		return true;
	}

	static bool ShowWhenShoppePresent()
	{
		return game::Shoppes::Read(game::Avatar::GetPosition()).has_value();
	}

	const std::vector<std::tuple<std::string, std::string, std::function<bool()>>> areaImages =
	{
		{AREA_MOVE_AHEAD, IMAGE_MOVE_AHEAD, AlwaysShow},
		{AREA_TURN_LEFT, IMAGE_TURN_LEFT, AlwaysShow},
		{AREA_TURN_RIGHT, IMAGE_TURN_RIGHT, AlwaysShow},
		{AREA_TRADE, IMAGE_TRADE, ShowWhenShoppePresent}
	};

	static void UpdateArrowImages(const std::string& area)
	{
		for (auto& areaImage : areaImages)
		{
			visuals::Images::SetVisible(LAYOUT_NAME, std::get<1>(areaImage), area == std::get<0>(areaImage) && std::get<2>(areaImage)());
		}
	}

	static std::string DetermineToolTip(const common::XY<size_t> worldPosition)
	{
		if (game::World::GetKnownState(worldPosition) == game::world::KnownState::UNKNOWN)
		{
			return CELL_UNKNOWN;
		}

		auto creature = game::Creatures::GetInstance(worldPosition);
		if (creature)
		{
			return creature.value().descriptor.name;
		}

		auto shoppe = game::Shoppes::Read(worldPosition);
		if (shoppe)
		{
			auto& descriptor = game::shoppe::GetDescriptor(shoppe.value());
			return descriptor.name;
		}

		return CELL_EMPTY;
	}

	static void UpdateMiniMapToolTip(const std::string& areaName, const common::XY<Sint32>& xy)
	{
		if (areaName == AREA_WORLD_MAP)
		{
			auto area = visuals::Areas::Get(LAYOUT_NAME, AREA_WORLD_MAP);
			auto worldSize = game::World::GetSize();
			size_t cellWidth = area.size.GetX() / worldSize.GetX();
			size_t cellHeight = area.size.GetY() / worldSize.GetY();
			size_t cellColumn = ((size_t)xy.GetX()) / cellWidth;
			size_t cellRow = ((size_t)xy.GetY()) / cellHeight;
			common::XY<size_t> worldPosition = { cellColumn , cellRow };
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_MAP_TOOL_TIP, DetermineToolTip(worldPosition));
		}
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

	static std::function<bool()> DoSomethingAndReturnTrue(std::function<std::optional<std::string>()> action)
	{
		return [action]() {
			common::audio::Sfx::Play(action());
			application::OnEnter::Handle(); 
			return true;
		};
	}

	static bool StartTrade()
	{
		EnterTrade();
		return HasShoppe();
	}

	const std::map<std::string, std::function<bool()>> mouseUpHandlers =
	{
		{ AREA_MOVE_AHEAD, DoSomethingAndReturnTrue(game::Avatar::MoveAhead)},
		{ AREA_TURN_LEFT, DoSomethingAndReturnTrue(game::Avatar::TurnLeft)},
		{ AREA_TURN_RIGHT, DoSomethingAndReturnTrue(game::Avatar::TurnRight)},
		{ AREA_TRADE, StartTrade}
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