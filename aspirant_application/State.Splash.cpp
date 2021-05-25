#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.Update.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Common.Audio.h"
namespace state::Splash
{
	const size_t TICKS_TOTAL = 3000;
	const std::string LAYOUT_NAME = "State.Splash";
	static size_t ticksLeft = TICKS_TOTAL;

	static void RunOutTimer()
	{
		ticksLeft = 0;
	}

	static void OnUpdate(const Uint32& ticks)
	{
		ticksLeft = (ticksLeft > ticks) ? (ticksLeft - ticks) : (0);
		if (ticksLeft == 0)
		{
			::application::UIState::Write(::UIState::MAIN_MENU);
			ticksLeft = TICKS_TOTAL;
		}
	}

	static bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8 buttons)
	{
		RunOutTimer();
		return true;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::SPLASH, common::audio::Mux::GoToTheme(common::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::SPLASH, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::SPLASH, RunOutTimer);
		::application::Renderer::SetRenderLayout(::UIState::SPLASH, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::SPLASH, OnUpdate);
	}
}
