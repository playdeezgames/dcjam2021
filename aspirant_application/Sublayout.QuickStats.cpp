#include <string>
#include <sstream>
#include "Game.Avatar.Statistics.h"
#include "Visuals.Texts.h"
#include "Application.Update.h"
#include "Visuals.Images.h"
#include <vector>
namespace sublayout::QuickStats
{
	const std::string LAYOUT_NAME = "Sublayout.QuickStats";
	const std::string HEALTH_IMAGE_ID = "HealthBar";
	const std::string HUNGER_IMAGE_ID = "HungerBar";
	const std::string HEALTH_SPRITE_PREFIX = "HealthBarForeground";
	const std::string HUNGER_SPRITE_PREFIX = "HungerBarForeground";

	static void UpdateHealth(const Uint32&)
	{
		std::stringstream ss;
		ss << HEALTH_SPRITE_PREFIX << game::avatar::Statistics::Read(game::avatar::Statistic::HEALTH);
		::graphics::Images::SetSprite(LAYOUT_NAME, HEALTH_IMAGE_ID, ss.str());
	}

	static void UpdateHunger(const Uint32&)
	{
		std::stringstream ss;
		ss << HUNGER_SPRITE_PREFIX << game::avatar::Statistics::Read(game::avatar::Statistic::HUNGER);
		::graphics::Images::SetSprite(LAYOUT_NAME, HUNGER_IMAGE_ID, ss.str());
	}

	const std::vector<::UIState> states =
	{
		::UIState::IN_PLAY_MAP,
		::UIState::IN_PLAY_INVENTORY,
		::UIState::IN_PLAY_STATUS,
		::UIState::IN_PLAY_COMBAT,
		::UIState::IN_PLAY_COMBAT_RESULT,
	};

	void Start()
	{
		for (auto state : states)
		{
			::application::Update::AddHandler(state, UpdateHealth);
			::application::Update::AddHandler(state, UpdateHunger);
		}
	}
}