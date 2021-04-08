#include "Application.UIState.h"
#include "Game.Avatar.h"
#include "Game.Creatures.h"
#include "Game.Avatar.Statistics.h"
#include "Application.Sounds.h"
#include "Common.Audio.h"
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
			common::audio::PlaySound(application::Sounds::DEAD_HUNTER);
			application::UIState::Write(::UIState::IN_PLAY_DEAD);
		}
		else if (game::Creatures::AnyLeft())
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
		else
		{
			common::audio::PlaySound(application::Sounds::EXIT);
			application::UIState::Write(::UIState::IN_PLAY_EXIT);
		}
	}
}