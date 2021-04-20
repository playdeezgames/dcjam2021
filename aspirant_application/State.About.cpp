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

	const std::map<Command, std::function<void()>> commandHandlers = 
	{
		{Command::GREEN, GoToMainMenu},
		{Command::RED, GoToMainMenu},
		{Command::BLUE, GoToMainMenu},
		{Command::YELLOW, GoToMainMenu}
	};

	void Start()
	{
		::application::Command::SetHandlers(::UIState::ABOUT, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::ABOUT, LAYOUT_NAME);
	}
}
