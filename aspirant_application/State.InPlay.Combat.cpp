#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
namespace state::in_play::Combat
{
	const std::string LAYOUT_NAME = "State.InPlay.Combat";

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
			application::UIState::Write(::UIState::LEAVE_PLAY);
			break;
		}
	}

	static void OnUpdate(const Uint32& ticks)
	{

	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_COMBAT, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_COMBAT, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT, OnUpdate);
	}
}