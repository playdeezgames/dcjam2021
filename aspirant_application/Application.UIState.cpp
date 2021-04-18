#include "Application.UIState.h"
#include "Game.Avatar.h"
#include "Game.Creatures.h"
#include "Game.Avatar.Statistics.h"
#include "Application.Sounds.h"
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

	std::optional<std::string> EnterGame()
	{
		std::optional<std::string> result = std::nullopt;
		if (game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HEALTH))
		{
			result = application::Sounds::DEAD_HUNTER;
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
			result = application::Sounds::EXIT;
			application::UIState::Write(::UIState::IN_PLAY_EXIT);
		}
		return result;
	}
}