#include "Application.UIState.h"
#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
namespace state::About
{
	const std::string LAYOUT_NAME = "State.About";

	static bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8 buttons)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::ABOUT, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::ABOUT, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(::UIState::ABOUT, LAYOUT_NAME);
	}
}
