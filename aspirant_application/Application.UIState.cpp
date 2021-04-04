#include "Application.UIState.h"
#include "Game.Avatar.h"
#include "Game.Creatures.h"
#include "Game.Avatar.Statistics.h"
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
		if (game::avatar::Statistics::IsDead())
		{
			application::UIState::Write(::UIState::IN_PLAY_DEAD);
		}
		else
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
}