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
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
namespace state::in_play::AvatarInventory
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarInventory";
	const std::string CONTROL_AVATAR_INVENTORY = "AvatarInventory";

	static void UseItem()
	{
		common::audio::Sfx::Play(game::avatar::Items::Use(graphics::AvatarInventory::GetItem()));
	}

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, []() { application::UIState::Write(::UIState::LEAVE_PLAY); }},
		{ ::Command::PREVIOUS, []() { graphics::AvatarInventory::ResetIndex(); application::UIState::Write(::UIState::IN_PLAY_FLOOR); }},
		{ ::Command::NEXT, []() { graphics::AvatarInventory::ResetIndex(); application::UIState::Write(::UIState::IN_PLAY_STATUS); }},
		{ ::Command::YELLOW, []() { graphics::AvatarInventory::ResetIndex(); application::UIState::Write(::UIState::IN_PLAY_STATUS); }},
		{ ::Command::UP, graphics::AvatarInventory::PreviousIndex },
		{ ::Command::DOWN, graphics::AvatarInventory::NextIndex },
		{ ::Command::RED, []() { game::avatar::Items::Drop(graphics::AvatarInventory::GetItem()); }},
		{ ::Command::GREEN, UseItem }
	};

	void OnMouseMotion(const common::XY<Sint32>& xy)
	{
		graphics::AvatarInventory::OnMouseMotion(LAYOUT_NAME, CONTROL_AVATAR_INVENTORY, xy);
	}

	bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8 buttons)
	{
		auto itemUsed = graphics::AvatarInventory::OnMouseButtonUp(LAYOUT_NAME, CONTROL_AVATAR_INVENTORY, xy, buttons);
		if (itemUsed.has_value())
		{
			UseItem();
		}
		return itemUsed.has_value();
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_INVENTORY, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_INVENTORY, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::IN_PLAY_INVENTORY, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_INVENTORY, LAYOUT_NAME);
	}
}