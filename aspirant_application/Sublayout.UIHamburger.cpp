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
#include "Common.Utility.h"
namespace sublayout::UIHamburger
{
	const std::string LAYOUT_NAME = "Sublayout.UIHamburger";
	const std::string AREA_UI_HAMBURGER = "UIHamburger";
	const std::string IMAGE_UI_HAMBURGER = "ui-hamburger";
	const std::string SPRITE_UI_HAMBURGER_NORMAL = "UIHamburgerNormal";
	const std::string SPRITE_UI_HAMBURGER_HOVER = "UIHamburgerHover";

	const std::vector<::UIState> states =
	{
		::UIState::IN_PLAY_MAP,
		::UIState::IN_PLAY_INVENTORY,
		::UIState::IN_PLAY_FLOOR,
		::UIState::IN_PLAY_STATUS,
		::UIState::IN_PLAY_COMBAT,
		::UIState::IN_PLAY_COMBAT_RESULT
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
	}

	static bool LeavePlay()
	{
		application::UIState::Write(::UIState::LEAVE_PLAY);
		return true;
	}

	const std::map<std::string, std::function<bool()>> mouseUpHandlers =
	{
		{AREA_UI_HAMBURGER, LeavePlay }
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