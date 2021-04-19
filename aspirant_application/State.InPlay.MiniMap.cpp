#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.Images.h"
#include "Graphics.Texts.h"
#include <vector>
#include <map>
#include "Game.Avatar.h"
#include "Game.World.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Item.h"
#include <sstream>
#include "States.h"
namespace state::in_play::MiniMap
{
	const std::string LAYOUT_NAME = "State.InPlay.MiniMap";

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, []() { application::UIState::Write(::UIState::LEAVE_PLAY); }},
		{ ::Command::LEFT, game::Avatar::TurnLeft },
		{ ::Command::RIGHT, game::Avatar::TurnRight },
		{ ::Command::UP, game::Avatar::MoveAhead },
		{ ::Command::DOWN, game::Avatar::MoveBack },
		{ ::Command::NEXT, []() { application::UIState::Write(::UIState::IN_PLAY_FLOOR); }},
		{ ::Command::YELLOW, []() {	application::UIState::Write(::UIState::IN_PLAY_FLOOR); }},
		{ ::Command::PREVIOUS, []() { application::UIState::Write(::UIState::IN_PLAY_STATUS); }}
	};

	static void OnCommand(const ::Command& command)
	{
		auto iter = commandHandlers.find(command);
		if (iter != commandHandlers.end())
		{
			iter->second();
		}
	}
	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_MAP, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_MAP, LAYOUT_NAME);
	}
}