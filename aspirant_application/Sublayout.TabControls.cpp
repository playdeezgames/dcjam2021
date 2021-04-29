#include <string>
#include <sstream>
#include "Game.Avatar.Statistics.h"
#include "Visuals.Texts.h"
#include "Application.Update.h"
#include "Visuals.Images.h"
#include <vector>
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
#include <map>
#include "Common.Utility.h"
#include "Visuals.Data.Colors.h"
namespace sublayout::TabControls
{
	const std::string LAYOUT_NAME = "Sublayout.TabControls";
	const std::string AREA_MAP = "Map";
	const std::string AREA_FLR = "Flr";
	const std::string AREA_INV = "Inv";
	const std::string AREA_STA = "Sta";
	const std::string TEXT_MAP = "Map";
	const std::string TEXT_FLR = "Flr";
	const std::string TEXT_INV = "Inv";
	const std::string TEXT_STA = "Sta";

	const std::vector<::UIState> states =
	{
		::UIState::IN_PLAY_MAP,
		::UIState::IN_PLAY_INVENTORY,
		::UIState::IN_PLAY_FLOOR,
		::UIState::IN_PLAY_STATUS
	};

	static void UpdateTabColor(const std::set<std::string>& areas, const std::string& areaId, const std::string textId)
	{
		if (areas.contains(areaId))
		{
			graphics::Texts::SetColor(LAYOUT_NAME, textId, graphics::data::Colors::HIGHLIGHT);
		}
		else
		{
			graphics::Texts::SetColor(LAYOUT_NAME, textId, graphics::data::Colors::NORMAL);
		}
	}

	const std::vector<std::tuple<std::string, std::string>> tabControls =
	{
		{ AREA_MAP, TEXT_MAP },
		{ AREA_FLR, TEXT_FLR },
		{ AREA_INV, TEXT_INV },
		{ AREA_STA, TEXT_STA }
	};

	static void OnMouseMotion(const common::XY<Sint32>& xy)
	{
		auto areas = graphics::Areas::Get(LAYOUT_NAME, xy);
		for (auto& tabControl : tabControls)
		{
			UpdateTabColor(areas, std::get<0>(tabControl), std::get<1>(tabControl));
		}
	}

	const std::map<std::string, std::function<bool()>> mouseUpHandlers =
	{
		{AREA_MAP, []() {application::UIState::Write(::UIState::IN_PLAY_MAP); return true; }},
		{AREA_FLR, []() {application::UIState::Write(::UIState::IN_PLAY_FLOOR); return true; }},
		{AREA_INV, []() {application::UIState::Write(::UIState::IN_PLAY_INVENTORY); return true; }},
		{AREA_STA, []() {application::UIState::Write(::UIState::IN_PLAY_STATUS); return true; }}
	};

	static bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8)
	{
		auto areas = graphics::Areas::Get(LAYOUT_NAME, xy);
		bool result = false;
		for (auto& area : areas)
		{
			result = result || common::Utility::Dispatch(mouseUpHandlers, area, false);
		}
		return false;
	}

	void Start()
	{
		for (auto state : states)
		{
			::application::MouseButtonUp::AddHandler(state, OnMouseButtonUp);
			::application::MouseMotion::AddHandler(state, OnMouseMotion);
		}
	}
}