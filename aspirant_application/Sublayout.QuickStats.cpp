#include <string>
#include <sstream>
#include "Game.Avatar.Statistics.h"
#include "Graphics.Texts.h"
#include "Application.Update.h"
#include "Graphics.Images.h"
namespace sublayout::QuickStats
{
	const std::string LAYOUT_NAME = "Sublayout.QuickStats";
	const std::string HUNGER_TEXT_ID = "Hunger";
	const std::string HEALTH_IMAGE_ID = "HealthBar";

	static void UpdateHealth(const Uint32&)
	{
		std::stringstream ss;
		ss << "HealthBarForeground" << game::avatar::Statistics::Read(game::avatar::Statistic::HEALTH);
		::graphics::Images::SetSprite(LAYOUT_NAME, HEALTH_IMAGE_ID, ss.str());
	}

	static void UpdateHunger(const Uint32&)
	{
		std::stringstream ss;
		ss << "Hunger: ";
		if (game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HUNGER))
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
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateHealth);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateHunger);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT, UpdateHealth);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT, UpdateHunger);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, UpdateHealth);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, UpdateHunger);
	}
}