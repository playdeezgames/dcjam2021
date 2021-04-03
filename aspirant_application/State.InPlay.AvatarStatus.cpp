#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
namespace state::in_play::AvatarStatus
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarStatus";

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
			application::UIState::Write(::UIState::LEAVE_PLAY);
			break;
		case ::Command::PREVIOUS:
			application::UIState::Write(::UIState::IN_PLAY_INVENTORY);
			break;
		case ::Command::NEXT:
		case ::Command::YELLOW:
			application::UIState::Write(::UIState::IN_PLAY_MAP);
			break;
		}
	}

	static void OnUpdate(const Uint32& ticks)
	{

	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_STATUS, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_STATUS, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, OnUpdate);
	}
}