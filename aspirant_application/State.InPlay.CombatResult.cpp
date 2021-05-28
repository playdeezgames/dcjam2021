#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Game.Combat.h"
#include "Visuals.Images.h"
#include "Visuals.CardSprites.h"
#include "Common.Audio.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Texts.h"
#include "Visuals.Data.Colors.h"
#include "Application.OnEnter.h"
#include "Visuals.Areas.h"
#include "Game.Audio.Mux.h"
namespace state::in_play::CombatResult
{
	const std::string LAYOUT_NAME = "State.InPlay.CombatResult";
	const std::string IMAGE_CURRENT_CARD = "CurrentCard";
	const std::string IMAGE_NEXT_CARD = "NextCard";
	const std::string AREA_CONTINUE = "Continue";
	const std::string TEXT_CONTINUE = "Continue";

	static void LeavePlay()
	{
		game::Combat::Advance(); 
		application::UIState::Write(::UIState::LEAVE_PLAY);
	}

	static void AdvanceCombat()
	{
		game::Combat::Advance(); 
		common::audio::Sfx::Play(application::UIState::EnterGame());
	}

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, LeavePlay },
		{ ::Command::GREEN, AdvanceCombat }
	};

	static void OnEnter()
	{
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_CURRENT_CARD, visuals::CardSprites::GetSpriteForCard(game::CombatDeck::GetCurrentCard()));
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_NEXT_CARD, visuals::CardSprites::GetSpriteForCard(game::CombatDeck::GetNextCard()));
	}

	static void OnMouseMotionInArea(const std::string& area, const common::XY<int>&)
	{
		visuals::Texts::SetColor(LAYOUT_NAME, TEXT_CONTINUE, (area == AREA_CONTINUE) ? (visuals::data::Colors::HIGHLIGHT) : (visuals::data::Colors::NORMAL));
	}

	static void OnMouseMotionOutsideAreas(const common::XY<int>&)
	{
		visuals::Texts::SetColor(LAYOUT_NAME, TEXT_CONTINUE, visuals::data::Colors::NORMAL);
	}

	static bool HandleContinue(const std::string& area)
	{
		if (area == AREA_CONTINUE)
		{
			AdvanceCombat();
			return true;
		}
		return false;
	}

	static bool OnMouseButtonUpInAreaa(const std::string& area)
	{
		return HandleContinue(area);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::BATTLE));
		application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInAreaa));
		application::MouseMotion::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideAreas));
		::application::Command::SetHandlers(::UIState::IN_PLAY_COMBAT_RESULT, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_COMBAT_RESULT, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, OnEnter);
	}
}