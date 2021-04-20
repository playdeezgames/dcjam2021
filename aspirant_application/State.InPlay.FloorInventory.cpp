#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.FloorInventory.h"
#include "Game.Item.h"
#include "Game.World.Items.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Items.h"
#include "Common.Utility.h"
namespace state::in_play::FloorInventory
{
	const std::string LAYOUT_NAME = "State.InPlay.FloorInventory";

	static void PickUpItem()
	{
		auto item = graphics::FloorInventory::GetItem();
		if (item)
		{
			auto inventory = game::world::Items::FloorInventory(game::Avatar::GetPosition());
			size_t amount = game::world::Items::Remove(game::Avatar::GetPosition(), *item, inventory[*item]);
			game::avatar::Items::Add(*item, amount);
		}
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, []() {application::UIState::Write(::UIState::LEAVE_PLAY); }},
		{ ::Command::PREVIOUS, []() {graphics::FloorInventory::ResetIndex(); application::UIState::Write(::UIState::IN_PLAY_MAP); } },
		{ ::Command::NEXT, []() {graphics::FloorInventory::ResetIndex(); application::UIState::Write(::UIState::IN_PLAY_INVENTORY); } },
		{ ::Command::YELLOW, []() {graphics::FloorInventory::ResetIndex(); application::UIState::Write(::UIState::IN_PLAY_INVENTORY); } },
		{ ::Command::UP, graphics::FloorInventory::PreviousIndex},
		{ ::Command::DOWN, graphics::FloorInventory::NextIndex},
		{ ::Command::GREEN, PickUpItem }
	};

	void Start()
	{
		::application::Command::SetHandlers(::UIState::IN_PLAY_FLOOR, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_FLOOR, LAYOUT_NAME);
	}
}