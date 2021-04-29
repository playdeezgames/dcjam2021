#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Visuals.FloorInventory.h"
#include "Game.Item.h"
#include "Game.World.Items.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Items.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
namespace state::in_play::FloorInventory
{
	const std::string LAYOUT_NAME = "State.InPlay.FloorInventory";
	const std::string CONTROL_FLOOR_INVENTORY = "FloorInventory";

	static void PickUpItem()
	{
		auto item = visuals::FloorInventory::GetItem();
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
		{ ::Command::BACK, []() {application::UIState::Write(::UIState::LEAVE_PLAY); }},
		{ ::Command::PREVIOUS, []() {visuals::FloorInventory::ResetIndex(); application::UIState::Write(::UIState::IN_PLAY_MAP); } },
		{ ::Command::NEXT, []() {visuals::FloorInventory::ResetIndex(); application::UIState::Write(::UIState::IN_PLAY_INVENTORY); } },
		{ ::Command::YELLOW, []() {visuals::FloorInventory::ResetIndex(); application::UIState::Write(::UIState::IN_PLAY_INVENTORY); } },
		{ ::Command::UP, visuals::FloorInventory::PreviousIndex},
		{ ::Command::DOWN, visuals::FloorInventory::NextIndex},
		{ ::Command::GREEN, PickUpItem }
	};

	void OnMouseMotion(const common::XY<Sint32>& xy)
	{
		visuals::FloorInventory::OnMouseMotion(LAYOUT_NAME, CONTROL_FLOOR_INVENTORY, xy);
	}

	bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8 buttons)
	{
		auto itemUsed = visuals::FloorInventory::OnMouseButtonUp(LAYOUT_NAME, CONTROL_FLOOR_INVENTORY, xy, buttons);
		if (itemUsed.has_value())
		{
			PickUpItem();
		}
		return itemUsed.has_value();
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_FLOOR, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_FLOOR, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::IN_PLAY_FLOOR, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_FLOOR, LAYOUT_NAME);
	}
}