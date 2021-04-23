#include <string>
#include "Graphics.Menus.h"
#include "Application.UIState.h"
#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Graphics.Areas.h"
namespace state::LeavePlay
{
	const std::string LAYOUT_NAME = "State.LeavePlay";
	const std::string MENU_ID = "LeavePlay";
	const std::string AREA_CONTINUE = "Continue";
	const std::string AREA_ABANDON = "Abandon";

	enum class LeavePlayItem
	{
		CONTINUE,
		ABANDON
	};

	const std::map<LeavePlayItem, std::function<void()>> activators =
	{
		{ LeavePlayItem::ABANDON, []() {::application::UIState::Write(::UIState::MAIN_MENU); }},
		{ LeavePlayItem::CONTINUE, []() {common::audio::Sfx::Play(application::UIState::EnterGame()); }}
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (LeavePlayItem)graphics::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, []() { graphics::Menus::Previous(LAYOUT_NAME, MENU_ID); }},
		{ ::Command::DOWN, []() { graphics::Menus::Next(LAYOUT_NAME, MENU_ID); }},
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, []() { ::application::UIState::Write(::UIState::MAIN_MENU); }},
		{ ::Command::RED, []() { ::application::UIState::Write(::UIState::MAIN_MENU); }}
	};

	static void SetCurrentMenuItem(LeavePlayItem item)
	{
		graphics::Menus::WriteValue(LAYOUT_NAME, MENU_ID, (int)item);
	}

	const std::map<std::string, LeavePlayItem> areaMenuItems =
	{
		{ AREA_CONTINUE,  LeavePlayItem::CONTINUE},
		{ AREA_ABANDON,  LeavePlayItem::ABANDON}
	};

	static void OnMouseMotion(const common::XY<Sint32>& xy)//TODO: make an MouseMotionArea handler?
	{
		auto areas = graphics::Areas::Get(LAYOUT_NAME, xy);
		for (auto& area : areas)
		{
			SetCurrentMenuItem(areaMenuItems.find(area)->second);
		}
	}

	static bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8)//TODO: duplicated code with other menus
	{
		auto areas = graphics::Areas::Get(LAYOUT_NAME, xy);
		if (!areas.empty())
		{
			ActivateItem();
			return true;
		}
		return false;
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::LEAVE_PLAY, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::LEAVE_PLAY, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::LEAVE_PLAY, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::LEAVE_PLAY, LAYOUT_NAME);
	}
}