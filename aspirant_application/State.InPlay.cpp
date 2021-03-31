#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
namespace state::InPlay
{
	const std::string LAYOUT_NAME = "State.InPlay";

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
		case ::Command::RED:
			application::UIState::Write(::UIState::MAIN_MENU);
			break;
		}
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY, LAYOUT_NAME);
	}
}