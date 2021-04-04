#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Game.Combat.h"
#include "Graphics.Images.h"
namespace state::in_play::CombatResult
{
	const std::string LAYOUT_NAME = "State.InPlay.CombatResult";
	const std::string CURRENT_CARD_IMAGE_ID = "CurrentCard";
	const std::string NEXT_CARD_IMAGE_ID = "NextCard";


	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
			game::Combat::Advance();
			application::UIState::Write(::UIState::LEAVE_PLAY);
			break;
		case ::Command::GREEN:
			game::Combat::Advance();
			application::UIState::EnterGame();
			break;
		}
	}

	static void OnUpdate(const Uint32& ticks)
	{
		graphics::Images::SetSprite(LAYOUT_NAME, CURRENT_CARD_IMAGE_ID, game::Combat::GetCurrentCard().sprite);
		graphics::Images::SetSprite(LAYOUT_NAME, NEXT_CARD_IMAGE_ID, game::Combat::GetNextCard().sprite);
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_COMBAT_RESULT, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_COMBAT_RESULT, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, OnUpdate);
	}
}