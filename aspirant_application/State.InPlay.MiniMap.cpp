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
#include "Game.Items.h"
#include <sstream>
namespace state::in_play::MiniMap
{
	const std::string HUNGER_TEXT_ID = "Hunger";
	const std::string HEALTH_TEXT_ID = "Health";
	const std::string LAYOUT_NAME = "State.InPlay.MiniMap";

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
		case ::Command::RED:
			application::UIState::Write(::UIState::LEAVE_PLAY);
			break;
		case ::Command::LEFT:
			game::Avatar::TurnLeft();
			break;
		case ::Command::RIGHT:
			game::Avatar::TurnRight();
			break;
		case ::Command::UP:
			game::Avatar::MoveAhead();
			break;
		case ::Command::DOWN:
			game::Avatar::MoveBack();
			break;
		}
	}

	static void UpdateHealth()
	{
		std::stringstream ss;
		ss << "Health: ";
		if (game::avatar::Statistics::IsDead())
		{
			ss << "DEAD!";
		}
		else
		{
			ss << game::avatar::Statistics::Read(game::avatar::Statistic::HEALTH) << "/" << game::avatar::Statistics::Maximum(game::avatar::Statistic::HEALTH);
		}
		::graphics::Texts::SetText(LAYOUT_NAME, HEALTH_TEXT_ID, ss.str());
	}

	static void UpdateHunger()
	{
		std::stringstream ss;
		ss << "Hunger: ";
		if (game::avatar::Statistics::IsStarving())
		{
			ss << "STARVING!";
		}
		else
		{
			ss << game::avatar::Statistics::Read(game::avatar::Statistic::HUNGER) << "/" << game::avatar::Statistics::Maximum(game::avatar::Statistic::HUNGER);
		}
		::graphics::Texts::SetText(LAYOUT_NAME, HUNGER_TEXT_ID, ss.str());
	}


	static void OnUpdate(const Uint32& ticks)
	{
		UpdateHealth();
		UpdateHunger();
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY, OnUpdate);
	}
}