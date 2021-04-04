#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Game.Combat.h"
#include "Graphics.Images.h"
#include "Graphics.Menus.h"
#include "Game.Avatar.h"
#include "Common.RNG.h"
namespace state::in_play::Combat
{
	const std::string LAYOUT_NAME = "State.InPlay.Combat";
	const std::string CURRENT_CARD_IMAGE_ID = "CurrentCard";
	const std::string COMBAT_MENU_ID = "Combat";

	enum class CombatMenuItem
	{
		HIGHER,
		LOWER,
		RUN_AWAY
	};

	static void Flee()
	{
		auto count = common::RNG::FromRange(0,(int)maze::Directions::All().size());
		while (count > 0)
		{
			game::Avatar::TurnRight();
			count--;
		}
		game::Avatar::MoveAhead();
	}

	static void OnActivateItem()
	{
		switch ((CombatMenuItem)graphics::Menus::Read(LAYOUT_NAME, COMBAT_MENU_ID).value())
		{
		case CombatMenuItem::HIGHER:
			game::Combat::Resolve(game::Combat::Guess::HIGHER);
			application::UIState::Write(::UIState::IN_PLAY_COMBAT_RESULT);
			break;
		case CombatMenuItem::LOWER:
			game::Combat::Resolve(game::Combat::Guess::LOWER);
			application::UIState::Write(::UIState::IN_PLAY_COMBAT_RESULT);
			break;
		case CombatMenuItem::RUN_AWAY:
			game::Combat::Resolve(std::nullopt);
			Flee();
			break;
		}
	}

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
			application::UIState::Write(::UIState::LEAVE_PLAY);
			break;
		case ::Command::UP:
			graphics::Menus::Previous(LAYOUT_NAME, COMBAT_MENU_ID);
			break;
		case ::Command::DOWN:
			graphics::Menus::Next(LAYOUT_NAME, COMBAT_MENU_ID);
			break;
		case ::Command::GREEN:
			OnActivateItem();
			break;
		}
	}

	static void OnUpdate(const Uint32& ticks)
	{
		auto& card = game::Combat::GetCurrentCard();
		graphics::Images::SetSprite(LAYOUT_NAME, CURRENT_CARD_IMAGE_ID, card.sprite);
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_COMBAT, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_COMBAT, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT, OnUpdate);
	}
}