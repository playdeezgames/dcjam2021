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
namespace state::in_play::AvatarInventory
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarInventory";

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, []() { application::UIState::Write(::UIState::LEAVE_PLAY); }},
		{ ::Command::PREVIOUS, []() { graphics::AvatarInventory::ResetIndex(); application::UIState::Write(::UIState::IN_PLAY_FLOOR); }},
		{ ::Command::NEXT, []() { graphics::AvatarInventory::ResetIndex(); application::UIState::Write(::UIState::IN_PLAY_STATUS); }},
		{ ::Command::YELLOW, []() { graphics::AvatarInventory::ResetIndex(); application::UIState::Write(::UIState::IN_PLAY_STATUS); }},
		{ ::Command::UP, graphics::AvatarInventory::PreviousIndex },
		{ ::Command::DOWN, graphics::AvatarInventory::NextIndex },
		{ ::Command::RED, []() { game::avatar::Items::Drop(graphics::AvatarInventory::GetItem()); }},
		{ ::Command::GREEN, []() { common::audio::Sfx::Play(game::avatar::Items::Use(graphics::AvatarInventory::GetItem())); }}
	};

	void Start()
	{
		::application::Command::SetHandlers(::UIState::IN_PLAY_INVENTORY, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_INVENTORY, LAYOUT_NAME);
	}
}