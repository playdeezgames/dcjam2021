#include <string>
#include <sstream>
#include "Game.Avatar.Statistics.h"
#include "Graphics.Texts.h"
#include "Application.Update.h"
namespace sublayout::QuickStats
{
	const std::string LAYOUT_NAME = "Sublayout.QuickStats";
	const std::string HUNGER_TEXT_ID = "Hunger";
	const std::string HEALTH_TEXT_ID = "Health";

	static void UpdateHealth(const Uint32&)
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

	static void UpdateHunger(const Uint32&)
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


	void Start()
	{
		::application::Update::AddHandler(::UIState::IN_PLAY_MAP, UpdateHealth);
		::application::Update::AddHandler(::UIState::IN_PLAY_MAP, UpdateHunger);
		::application::Update::AddHandler(::UIState::IN_PLAY_INVENTORY, UpdateHealth);
		::application::Update::AddHandler(::UIState::IN_PLAY_INVENTORY, UpdateHunger);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT, UpdateHealth);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT, UpdateHunger);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, UpdateHealth);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, UpdateHunger);
	}
}