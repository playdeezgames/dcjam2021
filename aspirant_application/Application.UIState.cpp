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
		if (game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HEALTH))
		{
			common::audio::Sfx::Play(application::Sounds::DEAD_HUNTER);//TODO: this is in the wrong layer
			application::UIState::Write(::UIState::IN_PLAY_DEAD);
		}
		else if (game::Creatures::AnyLeft())
		{
			if (game::Creatures::GetInstance(game::Avatar::GetPosition()))
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
			common::audio::Sfx::Play(application::Sounds::EXIT);//TODO: this is in the wrong layer
			application::UIState::Write(::UIState::IN_PLAY_EXIT);
		}
	}
}