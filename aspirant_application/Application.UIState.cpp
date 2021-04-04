#include "Application.UIState.h"
#include "Game.Avatar.h"
#include "Game.Creatures.h"
namespace application::UIState
{
	static ::UIState uiState = ::UIState::SPLASH;

	void Write(const ::UIState& state)
	{
		uiState = state;
	}

	const ::UIState& Read()
	{
		return uiState;
	}

	void EnterGame()
	{
		auto roomCreature = game::Creatures::Read(game::Avatar::GetPosition());
		if (roomCreature)
		{
			application::UIState::Write(::UIState::IN_PLAY_COMBAT);
		}
		else
		{
			application::UIState::Write(::UIState::IN_PLAY_MAP);
		}
	}
}