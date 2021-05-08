#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Game.Combat.h"
#include "Visuals.Images.h"
#include "Game.CombatDeck.h"
#include "Visuals.CardSprites.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Visuals.Areas.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Texts.h"
#include "Visuals.Data.Colors.h"
#include "Application.OnEnter.h"
#include "Visuals.Layouts.h"
namespace state::in_play::CombatResult
{
	const std::string LAYOUT_NAME = "State.InPlay.CombatResult";
	const std::string CURRENT_CARD_IMAGE_ID = "CurrentCard";
	const std::string NEXT_CARD_IMAGE_ID = "NextCard";
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
		visuals::Images::SetSprite(LAYOUT_NAME, CURRENT_CARD_IMAGE_ID, visuals::CardSprites::GetSpriteForCard(game::CombatDeck::GetCurrentCard()));
		visuals::Images::SetSprite(LAYOUT_NAME, NEXT_CARD_IMAGE_ID, visuals::CardSprites::GetSpriteForCard(game::CombatDeck::GetNextCard()));
	}

	static void OnMouseMotion(const common::XY<Sint32>& xy)
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		visuals::Texts::SetColor(LAYOUT_NAME, TEXT_CONTINUE, (areas.contains(AREA_CONTINUE)) ? (visuals::data::Colors::HIGHLIGHT) : (visuals::data::Colors::NORMAL));
	}

	static bool HandleContinue(const std::set<std::string>& areas)
	{
		if (areas.contains(AREA_CONTINUE))
		{
			AdvanceCombat();
			return true;
		}
		return false;
	}

	static bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8)
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		return HandleContinue(areas);
	}

	void Start()
	{
		application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, OnMouseButtonUp);
		application::MouseMotion::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::IN_PLAY_COMBAT_RESULT, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_COMBAT_RESULT, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, OnEnter);
	}
}