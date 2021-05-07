#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Visuals.AvatarInventory.h"
#include "Game.Avatar.Items.h"
#include "Game.Item.h"
#include "Game.World.Items.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Statistics.h"
#include "Common.Audio.h"
#include "Application.Sounds.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Visuals.Layouts.h"
namespace state::in_play::AvatarInventory
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarInventory";
	const std::string CONTROL_AVATAR_INVENTORY = "AvatarInventory";

	static void UseItem()
	{
		common::audio::Sfx::Play(game::avatar::Items::Use(visuals::AvatarInventory::GetItem(LAYOUT_NAME, CONTROL_AVATAR_INVENTORY)));
	}

	static void DropItem()
	{
		game::avatar::Items::Drop(visuals::AvatarInventory::GetItem(LAYOUT_NAME, CONTROL_AVATAR_INVENTORY));
	}

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{ ::Command::PREVIOUS, application::UIState::GoTo(::UIState::IN_PLAY_FLOOR) },
		{ ::Command::NEXT, application::UIState::GoTo(::UIState::IN_PLAY_STATUS) },
		{ ::Command::YELLOW, application::UIState::GoTo(::UIState::IN_PLAY_STATUS) },
		{ ::Command::UP, visuals::AvatarInventory::GoToPreviousIndex(LAYOUT_NAME, CONTROL_AVATAR_INVENTORY) },
		{ ::Command::DOWN, visuals::AvatarInventory::GoToNextIndex(LAYOUT_NAME, CONTROL_AVATAR_INVENTORY)    },
		{ ::Command::RED, DropItem },
		{ ::Command::GREEN, UseItem }
	};

	void OnMouseMotion(const common::XY<Sint32>& xy)
	{
		visuals::AvatarInventory::OnMouseMotion(LAYOUT_NAME, CONTROL_AVATAR_INVENTORY, xy);
	}

	bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8 buttons)
	{
		auto itemUsed = visuals::AvatarInventory::OnMouseButtonUp(LAYOUT_NAME, CONTROL_AVATAR_INVENTORY, xy, buttons);
		if (itemUsed.has_value())
		{
			UseItem();
		}
		return itemUsed.has_value();
	}

	static void OnEnter()
	{
		visuals::Layouts::InitializeLayout(LAYOUT_NAME);
		visuals::AvatarInventory::ResetIndex(LAYOUT_NAME, CONTROL_AVATAR_INVENTORY);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_INVENTORY, OnEnter );
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_INVENTORY, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_INVENTORY, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::IN_PLAY_INVENTORY, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_INVENTORY, LAYOUT_NAME);
	}
}