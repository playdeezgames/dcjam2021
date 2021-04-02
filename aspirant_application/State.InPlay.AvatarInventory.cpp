#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
namespace state::in_play::AvatarInventory
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarInventory";

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
		case ::Command::RED:
			application::UIState::Write(::UIState::LEAVE_PLAY);
			break;
		case ::Command::PREVIOUS:
			application::UIState::Write(::UIState::IN_PLAY_FLOOR);
			break;
		case ::Command::NEXT:
		case ::Command::YELLOW:
			application::UIState::Write(::UIState::IN_PLAY_STATUS);
			break;
		}
	}

	static void OnUpdate(const Uint32& ticks)
	{

	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_INVENTORY, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_INVENTORY, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_INVENTORY, OnUpdate);
	}
}