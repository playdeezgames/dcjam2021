#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.Images.h"
#include <vector>
#include <map>
namespace state::InPlay
{
	const std::string LAYOUT_NAME = "State.InPlay";
	const std::string LEFT_SIDE = "LeftSide";
	const std::string AHEAD = "Ahead";
	const std::string RIGHT_SIDE = "RightSide";

	static std::vector<std::map<std::string, std::string>> items =
	{
		{
			{ LEFT_SIDE, "LeftWall"},
			{ AHEAD, "AheadDoor"},
			{ RIGHT_SIDE, "RightWall"},
		},
		{
			{ LEFT_SIDE, "LeftDoor"},
			{ AHEAD, "AheadWall"},
			{ RIGHT_SIDE, "RightWall"},
		},
		{
			{ LEFT_SIDE, "LeftWall"},
			{ AHEAD, "AheadWall"},
			{ RIGHT_SIDE, "RightWall"},
		},
		{
			{ LEFT_SIDE, "LeftWall"},
			{ AHEAD, "AheadWall"},
			{ RIGHT_SIDE, "RightDoor"},
		}
	};

	static size_t index = 0;

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
		case ::Command::RED:
			application::UIState::Write(::UIState::MAIN_MENU);
		case ::Command::LEFT:
			index = (index + 3) % 4;
			break;
		case ::Command::RIGHT:
			index = (index + 1) % 4;
			break;
		}
	}

	static void OnUpdate(const Uint32& ticks)
	{
		for (auto& item : items[index])
		{
			::graphics::Images::SetSprite(LAYOUT_NAME, item.first, item.second);
		}
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY, OnUpdate);
	}
}