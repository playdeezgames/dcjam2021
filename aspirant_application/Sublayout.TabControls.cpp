#include "Visuals.Texts.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
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
	const std::string NO_TAB = "";

	const std::vector<::UIState> states =
	{
		::UIState::IN_PLAY_MAP,
		::UIState::IN_PLAY_INVENTORY,
		::UIState::IN_PLAY_FLOOR,
		::UIState::IN_PLAY_STATUS
	};

	static void UpdateTabColor(const std::string& area, const std::string& areaId, const std::string textId)
	{
		visuals::Texts::SetColor(LAYOUT_NAME, textId, (area == areaId) ? (visuals::data::Colors::HIGHLIGHT) : (visuals::data::Colors::NORMAL));
	}

	const std::vector<std::tuple<std::string, std::string>> tabControls =
	{
		{ AREA_MAP, TEXT_MAP },
		{ AREA_FLR, TEXT_FLR },
		{ AREA_INV, TEXT_INV },
		{ AREA_STA, TEXT_STA }
	};

	static void UpdateTabColors(const std::string& area)
	{
		for (auto& tabControl : tabControls)
		{
			UpdateTabColor(area, std::get<0>(tabControl), std::get<1>(tabControl));
		}
	}

	static void OnMouseMotionInArea(const std::string& area, const common::XY<int>&)
	{
		UpdateTabColors(area);
	}

	static void OnMouseMotionOutsideAreas(const common::XY<int>&)
	{
		UpdateTabColors(NO_TAB);
	}

	static std::function<bool()> GoToStateAndReturnTrue(::UIState state)
	{
		return [state]() {application::UIState::Write(state); return true; };
	}

	const std::map<std::string, std::function<bool()>> mouseUpHandlers =
	{
		{AREA_MAP, GoToStateAndReturnTrue(::UIState::IN_PLAY_MAP)},
		{AREA_FLR, GoToStateAndReturnTrue(::UIState::IN_PLAY_FLOOR)},
		{AREA_INV, GoToStateAndReturnTrue(::UIState::IN_PLAY_INVENTORY)},
		{AREA_STA, GoToStateAndReturnTrue(::UIState::IN_PLAY_STATUS)}
	};

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		return common::Utility::Dispatch(mouseUpHandlers, area, false);
	}

	void Start()
	{
		for (auto state : states)
		{
			::application::MouseButtonUp::AddHandler(state, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
			::application::MouseMotion::AddHandler(state, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideAreas));
		}
	}
}