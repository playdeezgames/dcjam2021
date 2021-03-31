#include "Application.UIState.h"
#include "Application.Renderer.h"
#include "Application.Command.h"
namespace state::About
{
	const std::string LAYOUT_NAME = "State.About";

	static void OnCommand(const ::Command& command)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::ABOUT, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::ABOUT, LAYOUT_NAME);
	}
}
