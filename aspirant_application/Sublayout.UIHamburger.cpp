#include <string>
#include <sstream>
#include "Game.Avatar.Statistics.h"
#include "Graphics.Texts.h"
#include "Application.Update.h"
#include "Graphics.Images.h"
#include <vector>
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Graphics.Areas.h"
#include <map>
namespace sublayout::UIHamburger
{
	const std::string LAYOUT_NAME = "Sublayout.UIHamburger";
	const std::string AREA_UI_HAMBURGER = "UIHamburger";
	const std::string AREA_MAP = "Map";
	const std::string AREA_FLR = "Flr";
	const std::string AREA_INV = "Inv";
	const std::string AREA_STA = "Sta";
	const std::string TEXT_MAP = "Map";
	const std::string TEXT_FLR = "Flr";
	const std::string TEXT_INV = "Inv";
	const std::string TEXT_STA = "Sta";
	const std::string IMAGE_UI_HAMBURGER = "ui-hamburger";
	const std::string SPRITE_UI_HAMBURGER_NORMAL = "UIHamburgerNormal";
	const std::string SPRITE_UI_HAMBURGER_HOVER = "UIHamburgerHover";
	const std::string COLOR_HIGHLIGHT = "Yellow";
	const std::string COLOR_NORMAL = "Gray";

	const std::vector<::UIState> states =
	{
		::UIState::IN_PLAY_MAP,
		::UIState::IN_PLAY_INVENTORY,
		::UIState::IN_PLAY_FLOOR,
		::UIState::IN_PLAY_STATUS,
		::UIState::IN_PLAY_COMBAT,
		::UIState::IN_PLAY_COMBAT_RESULT,
	};

	static void UpdateTabColor(const std::set<std::string>& areas, const std::string& areaId, const std::string textId)
	{
		if (areas.contains(areaId))
		{
			graphics::Texts::SetColor(LAYOUT_NAME, textId, COLOR_HIGHLIGHT);
		}
		else
		{
			graphics::Texts::SetColor(LAYOUT_NAME, textId, COLOR_NORMAL);
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
		if (areas.contains(AREA_UI_HAMBURGER))
		{
			graphics::Images::SetSprite(LAYOUT_NAME, IMAGE_UI_HAMBURGER, SPRITE_UI_HAMBURGER_HOVER);
		}
		else
		{
			graphics::Images::SetSprite(LAYOUT_NAME, IMAGE_UI_HAMBURGER, SPRITE_UI_HAMBURGER_NORMAL);
		}
		for (auto& tabControl : tabControls)
		{
			UpdateTabColor(areas, std::get<0>(tabControl), std::get<1>(tabControl));
		}
	}

	static bool LeavePlay()
	{
		application::UIState::Write(::UIState::LEAVE_PLAY);
		return true;
	}

	const std::map<std::string, std::function<bool()>> mouseUpHandlers =
	{
		{AREA_UI_HAMBURGER, LeavePlay },
		{AREA_MAP, []() {application::UIState::Write(::UIState::IN_PLAY_MAP); return true; }},
		{AREA_FLR, []() {application::UIState::Write(::UIState::IN_PLAY_FLOOR); return true; }},
		{AREA_INV, []() {application::UIState::Write(::UIState::IN_PLAY_INVENTORY); return true; }},
		{AREA_STA, []() {application::UIState::Write(::UIState::IN_PLAY_STATUS); return true; }}
	};

	static bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8)
	{
		auto areas = graphics::Areas::Get(LAYOUT_NAME, xy);
		for (auto& area : areas)
		{
			auto iter = mouseUpHandlers.find(area);
			if (iter != mouseUpHandlers.end())
			{
				return iter->second();
			}
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