#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Graphics.Layouts.h"
#include "Graphics.Menus.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Graphics.Areas.h"
namespace state::ConfirmQuit
{
	const std::string LAYOUT_NAME = "State.ConfirmQuit";
	const std::string MENU_ID = "ConfirmQuit";
	const std::string AREA_NO = "No";
	const std::string AREA_YES = "Yes";

	enum class ConfirmQuitItem
	{
		NO,
		YES
	};

	static void GoToMainMenu()
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
	}

	static void Quit()
	{
		::application::UIState::Write(::UIState::QUIT);
	}

	const std::map<ConfirmQuitItem, std::function<void()>> activators =
	{
		{ ConfirmQuitItem::NO, GoToMainMenu },
		{ ConfirmQuitItem::YES, Quit }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (ConfirmQuitItem)graphics::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, graphics::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, graphics::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, GoToMainMenu },
		{ ::Command::RED, GoToMainMenu }
	};

	static void SetCurrentMenuItem(ConfirmQuitItem item)
	{
		graphics::Menus::WriteValue(LAYOUT_NAME, MENU_ID, (int)item);
	}

	const std::map<std::string, ConfirmQuitItem> areaMenuItems =
	{
		{ AREA_NO,  ConfirmQuitItem::NO},
		{ AREA_YES,  ConfirmQuitItem::YES}
	};

	static void OnMouseMotion(const common::XY<Sint32>& xy)//TODO: make an MouseMotionArea handler?
	{
		auto areas = graphics::Areas::Get(LAYOUT_NAME, xy);
		for (auto& area : areas)
		{
			SetCurrentMenuItem(areaMenuItems.find(area)->second);
		}
	}

	bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8)//TODO: duplicated code with other menus
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
		::application::MouseButtonUp::AddHandler(::UIState::CONFIRM_QUIT, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::CONFIRM_QUIT, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::CONFIRM_QUIT, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::CONFIRM_QUIT, LAYOUT_NAME);
	}
}