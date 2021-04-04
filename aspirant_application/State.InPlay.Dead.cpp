#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.Update.h"
#include "Application.UIState.h"
namespace state::in_play::Dead
{
	const std::string LAYOUT_NAME = "State.InPlay.Dead";

	static void OnCommand(const ::Command& command)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_DEAD, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_DEAD, LAYOUT_NAME);
	}
}
