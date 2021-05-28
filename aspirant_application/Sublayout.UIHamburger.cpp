#include "Visuals.Images.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
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
		::UIState::IN_PLAY_COMBAT_RESULT,
		::UIState::IN_PLAY_TRADE
	};

	static void OnMouseMotionInArea(const std::string& area, const common::XY<int>&)
	{
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_UI_HAMBURGER, SPRITE_UI_HAMBURGER_HOVER);
	}

	static void OnMouseMotionOutsideAreas(const common::XY<int>&)
	{
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_UI_HAMBURGER, SPRITE_UI_HAMBURGER_NORMAL);
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