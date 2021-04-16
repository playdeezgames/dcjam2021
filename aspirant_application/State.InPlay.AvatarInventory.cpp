#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.AvatarInventory.h"
#include "Game.Avatar.Items.h"
#include "Game.Item.h"
#include "Game.World.Items.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Statistics.h"
#include "Common.Audio.h"
#include "Application.Sounds.h"
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
		case ::Command::RED:
			game::avatar::Items::Drop(graphics::AvatarInventory::GetItem());
			break;
		case ::Command::GREEN:
			auto sfx = game::avatar::Items::Use(graphics::AvatarInventory::GetItem());
			if (sfx)
			{
				common::audio::Sfx::Play(sfx.value());
			}
			break;
		}
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_INVENTORY, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_INVENTORY, LAYOUT_NAME);
	}
}