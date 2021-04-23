#include "Application.UIState.h"
#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
namespace state::About
{
	const std::string LAYOUT_NAME = "State.About";

	static void GoToMainMenu()
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
	}

	bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8 buttons)
	{
		GoToMainMenu();
		return true;
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::ABOUT, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::ABOUT, GoToMainMenu);
		::application::Renderer::SetRenderLayout(::UIState::ABOUT, LAYOUT_NAME);
	}
}
