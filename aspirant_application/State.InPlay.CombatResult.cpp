#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Game.Combat.h"
#include "Graphics.Images.h"
#include "Game.CombatDeck.h"
#include "Graphics.CardSprites.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
namespace state::in_play::CombatResult
{
	const std::string LAYOUT_NAME = "State.InPlay.CombatResult";
	const std::string CURRENT_CARD_IMAGE_ID = "CurrentCard";
	const std::string NEXT_CARD_IMAGE_ID = "NextCard";

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, []() { game::Combat::Advance(); application::UIState::Write(::UIState::LEAVE_PLAY); }},
		{ ::Command::GREEN, []() { game::Combat::Advance(); common::audio::Sfx::Play(application::UIState::EnterGame()); }}
	};

	static void OnCommand(const ::Command& command)
	{
		common::Utility::Dispatch(commandHandlers, command);
	}

	static void OnUpdate(const Uint32& ticks)
	{
		graphics::Images::SetSprite(LAYOUT_NAME, CURRENT_CARD_IMAGE_ID, graphics::CardSprites::GetSpriteForCard(game::CombatDeck::GetCurrentCard()));
		graphics::Images::SetSprite(LAYOUT_NAME, NEXT_CARD_IMAGE_ID, graphics::CardSprites::GetSpriteForCard(game::CombatDeck::GetNextCard()));
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_COMBAT_RESULT, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_COMBAT_RESULT, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, OnUpdate);
	}
}