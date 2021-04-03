#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.FloorInventory.h"
#include "Game.Items.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Items.h"
namespace state::in_play::FloorInventory
{
	const std::string LAYOUT_NAME = "State.InPlay.FloorInventory";

	static void PickUpItem()
	{
		auto item = graphics::FloorInventory::GetItem();
		if (item)
		{
			size_t amount = game::Items::Remove(*item, 1, game::Avatar::GetPosition());
			game::avatar::Items::Add(*item, amount);
		}
	}

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
			application::UIState::Write(::UIState::LEAVE_PLAY);
			break;
		case ::Command::PREVIOUS:
			graphics::FloorInventory::ResetIndex();
			application::UIState::Write(::UIState::IN_PLAY_MAP);
			break;
		case ::Command::NEXT:
		case ::Command::YELLOW:
			graphics::FloorInventory::ResetIndex();
			application::UIState::Write(::UIState::IN_PLAY_INVENTORY);
			break;
		case ::Command::UP:
			graphics::FloorInventory::PreviousIndex();
			break;
		case ::Command::DOWN:
			graphics::FloorInventory::NextIndex();
			break;
		case ::Command::GREEN:
			PickUpItem();
			break;
		}
	}

	static void OnUpdate(const Uint32& ticks)
	{

	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_FLOOR, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_FLOOR, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_FLOOR, OnUpdate);
	}
}