#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.AvatarInventory.h"
namespace state::in_play::AvatarInventory
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarInventory";

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
			application::UIState::Write(::UIState::LEAVE_PLAY);
			break;
		case ::Command::PREVIOUS:
			graphics::AvatarInventory::ResetIndex();
			application::UIState::Write(::UIState::IN_PLAY_FLOOR);
			break;
		case ::Command::NEXT:
		case ::Command::YELLOW:
			graphics::AvatarInventory::ResetIndex();
			application::UIState::Write(::UIState::IN_PLAY_STATUS);
			break;
		case ::Command::UP:
			graphics::AvatarInventory::PreviousIndex();
			break;
		case ::Command::DOWN:
			graphics::AvatarInventory::NextIndex();
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