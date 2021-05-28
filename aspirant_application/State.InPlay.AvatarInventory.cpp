#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Visuals.AvatarInventory.h"
#include "Game.Avatar.Items.h"
#include "Common.Audio.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
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
		application::OnEnter::Handle();
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

	void OnMouseMotion(const common::XY<int>& xy)
	{
		visuals::AvatarInventory::OnMouseMotion(LAYOUT_NAME, CONTROL_AVATAR_INVENTORY, xy);
	}

	bool OnMouseButtonUp(const common::XY<int>& xy, unsigned char buttons)
	{
		auto itemUsed = visuals::AvatarInventory::OnMouseButtonUp(LAYOUT_NAME, CONTROL_AVATAR_INVENTORY, xy, buttons);
		if (itemUsed.has_value())
		{
			if (buttons == SDL_BUTTON_RIGHT)
			{
				DropItem();
			}
			else
			{
				UseItem();
			}
		}
		return itemUsed.has_value();
	}

	static void OnEnter()
	{
		visuals::AvatarInventory::ResetIndex(LAYOUT_NAME, CONTROL_AVATAR_INVENTORY);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_INVENTORY, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_INVENTORY, OnEnter );
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_INVENTORY, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_INVENTORY, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::IN_PLAY_INVENTORY, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_INVENTORY, LAYOUT_NAME);
	}
}