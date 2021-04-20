#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.Texts.h"
#include "Game.Avatar.Items.h"
#include <sstream>
namespace state::in_play::Exit
{
	const std::string LAYOUT_NAME = "State.InPlay.Exit";
	const std::string JOOLS_TEXT_ID = "Jools";

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

	static void OnUpdate(const Uint32&)
	{
		std::stringstream ss;
		ss << "You collected " << game::avatar::Items::Read(5) << " jools!";
		graphics::Texts::SetText(LAYOUT_NAME, JOOLS_TEXT_ID, ss.str());
	}

	void Start()
	{
		::application::Command::SetHandlers(::UIState::IN_PLAY_EXIT, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_EXIT, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_EXIT, OnUpdate);
	}
}
