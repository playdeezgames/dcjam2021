#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Visuals.Menus.h"
#include "Game.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
namespace state::Start
{
	const std::string LAYOUT_NAME = "State.Start";
	const std::string MENU_ID = "Start";
	const std::string AREA_NEW_GAME_EASY = "NewGameEasy";
	const std::string AREA_NEW_GAME_NORMAL = "NewGameNormal";
	const std::string AREA_NEW_GAME_HARD = "NewGameHard";
	const std::string AREA_CONTINUE_GAME = "ContinueGame";
	const std::string AREA_BACK = "Back";

	enum class StartGameItem
	{
		CONTINUE_GAME,
		NEW_GAME_EASY,
		NEW_GAME_NORMAL,
		NEW_GAME_HARD,
		BACK
	};

	static std::function<void()> NewGame(const game::Difficulty& difficulty)
	{
		return [difficulty]()
		{
			game::Reset(difficulty);
			common::audio::Sfx::Play(application::UIState::EnterGame());
		};
	}

	const std::map<StartGameItem, std::function<void()>> activators =
	{
		{ StartGameItem::NEW_GAME_EASY, NewGame(game::Difficulty::EASY) },
		{ StartGameItem::NEW_GAME_NORMAL, NewGame(game::Difficulty::NORMAL) },
		{ StartGameItem::NEW_GAME_HARD, NewGame(game::Difficulty::HARD) },
		{ StartGameItem::CONTINUE_GAME, ::application::UIState::GoTo(::UIState::LOAD_GAME) },
		{ StartGameItem::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (StartGameItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::GREEN, ActivateItem }
	};

	static void SetCurrentMenuItem(StartGameItem item)
	{
		visuals::Menus::WriteIndex(LAYOUT_NAME, MENU_ID, (int)item);
	}

	const std::map<std::string, StartGameItem> areaMenuItems =
	{
		{ AREA_NEW_GAME_EASY,  StartGameItem::NEW_GAME_EASY},
		{ AREA_NEW_GAME_NORMAL,  StartGameItem::NEW_GAME_NORMAL},
		{ AREA_NEW_GAME_HARD,  StartGameItem::NEW_GAME_HARD},
		{ AREA_CONTINUE_GAME,  StartGameItem::CONTINUE_GAME},
		{ AREA_BACK,  StartGameItem::BACK}
	};

	static void OnMouseMotionInArea(const std::string& area, const common::XY<int>&)
	{
		SetCurrentMenuItem(areaMenuItems.find(area)->second);
	}

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		ActivateItem();
		return true;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::START_GAME, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::START_GAME, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(::UIState::START_GAME, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea));
		::application::Command::SetHandlers(::UIState::START_GAME, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::START_GAME, LAYOUT_NAME);
	}
}