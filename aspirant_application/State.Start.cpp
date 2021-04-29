#include "Application.Renderer.h"
#include "Visuals.Layouts.h"
#include "Application.Command.h"
#include "Application.UIState.h"
#include "Visuals.Menus.h"
#include "Game.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
namespace state::Start
{
	const std::string LAYOUT_NAME = "State.Start";
	const std::string MENU_ID = "Start";
	const std::string AREA_NEW_GAME = "NewGame";
	const std::string AREA_CONTINUE_GAME = "ContinueGame";
	const std::string AREA_BACK = "Back";

	enum class StartGameItem
	{
		NEW_GAME,
		CONTINUE_GAME,
		BACK
	};

	static void NewGame()
	{
		game::Reset();
		common::audio::Sfx::Play(application::UIState::EnterGame());
	}

	static void ContinueGame()
	{
		::application::UIState::Write(::UIState::LOAD_GAME);
	}

	static void GoBack()
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
	}

	const std::map<StartGameItem, std::function<void()>> activators =
	{
		{ StartGameItem::NEW_GAME, NewGame },
		{ StartGameItem::CONTINUE_GAME, ContinueGame },
		{ StartGameItem::BACK, GoBack }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (StartGameItem)visuals::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::BACK, GoBack },
		{ ::Command::RED, GoBack },
		{ ::Command::GREEN, ActivateItem }
	};

	static void SetCurrentMenuItem(StartGameItem item)
	{
		visuals::Menus::WriteValue(LAYOUT_NAME, MENU_ID, (int)item);
	}

	const std::map<std::string, StartGameItem> areaMenuItems =
	{
		{ AREA_NEW_GAME,  StartGameItem::NEW_GAME},
		{ AREA_CONTINUE_GAME,  StartGameItem::CONTINUE_GAME},
		{ AREA_BACK,  StartGameItem::BACK}
	};

	static void OnMouseMotion(const common::XY<Sint32>& xy)//TODO: make an MouseMotionArea handler?
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		for (auto& area : areas)
		{
			SetCurrentMenuItem(areaMenuItems.find(area)->second);
		}
	}

	static bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8)//TODO: duplicated code with other menus
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		if (!areas.empty())
		{
			ActivateItem();
			return true;
		}
		return false;
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::START_GAME, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::START_GAME, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::START_GAME, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::START_GAME, LAYOUT_NAME);
	}
}