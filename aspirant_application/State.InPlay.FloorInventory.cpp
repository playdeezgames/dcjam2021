#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.OnEnter.h"
#include "Visuals.AvatarInventory.h"
#include "Game.World.Items.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Items.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Game.Audio.Mux.h"
namespace state::in_play::FloorInventory
{
	const std::string LAYOUT_NAME = "State.InPlay.FloorInventory";
	const std::string CONTROL_FLOOR_INVENTORY = "FloorInventory";

	static void PickUpItem()
	{
		auto item = visuals::AvatarInventory::GetItem(LAYOUT_NAME, CONTROL_FLOOR_INVENTORY);
		if (item)
		{
			//TODO: this is duplicated code
			auto inventory = game::world::Items::FloorInventory(game::Avatar::GetPosition());
			size_t amount = game::world::Items::Remove(game::Avatar::GetPosition(), *item, inventory[*item]);
			game::avatar::Items::Add(*item, amount);
		}
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{ ::Command::PREVIOUS, application::UIState::GoTo(::UIState::IN_PLAY_MAP) },
		{ ::Command::NEXT, application::UIState::GoTo(::UIState::IN_PLAY_INVENTORY) },
		{ ::Command::YELLOW, application::UIState::GoTo(::UIState::IN_PLAY_INVENTORY) },
		{ ::Command::UP, visuals::AvatarInventory::GoToPreviousIndex(LAYOUT_NAME, CONTROL_FLOOR_INVENTORY)},
		{ ::Command::DOWN, visuals::AvatarInventory::GoToNextIndex(LAYOUT_NAME, CONTROL_FLOOR_INVENTORY)},
		{ ::Command::GREEN, PickUpItem }
	};

	void OnMouseMotion(const common::XY<int>& xy)
	{
		visuals::AvatarInventory::OnMouseMotion(LAYOUT_NAME, CONTROL_FLOOR_INVENTORY, xy);
	}

	bool OnMouseButtonUp(const common::XY<int>& xy, unsigned char buttons)
	{
		auto itemUsed = visuals::AvatarInventory::OnMouseButtonUp(LAYOUT_NAME, CONTROL_FLOOR_INVENTORY, xy, buttons);
		if (itemUsed.has_value())
		{
			PickUpItem();
		}
		return itemUsed.has_value();
	}

	static void OnEnter()
	{
		visuals::AvatarInventory::ResetIndex(LAYOUT_NAME, CONTROL_FLOOR_INVENTORY);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_FLOOR, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_FLOOR, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_FLOOR, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_FLOOR, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::IN_PLAY_FLOOR, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_FLOOR, LAYOUT_NAME);
	}
}