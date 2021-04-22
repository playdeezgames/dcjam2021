#include "Application.UIState.h"
#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Common.Utility.h"
namespace state::About
{
	const std::string LAYOUT_NAME = "State.About";

	static void GoToMainMenu()
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::ABOUT, GoToMainMenu);
		::application::Renderer::SetRenderLayout(::UIState::ABOUT, LAYOUT_NAME);
	}
}
