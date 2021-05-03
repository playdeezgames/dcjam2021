#include <string>
#include <sstream>
#include "Game.Avatar.Statistics.h"
#include "Visuals.Texts.h"
#include "Application.Update.h"
#include "Visuals.Images.h"
#include <vector>
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
#include <sstream>
namespace sublayout::QuickStats
{
	const std::string LAYOUT_NAME = "Sublayout.QuickStats";
	const std::string HEALTH_IMAGE_ID = "HealthBar";
	const std::string HUNGER_IMAGE_ID = "HungerBar";
	const std::string HEALTH_SPRITE_PREFIX = "HealthBarForeground";
	const std::string HUNGER_SPRITE_PREFIX = "HungerBarForeground";
	const std::string AREA_HEALTH = "Health";
	const std::string AREA_HUNGER = "Hunger";
	const std::string TEXT_TOOL_TOP = "ToolTip";

	static void UpdateHealth(const Uint32&)
	{
		std::stringstream ss;
		ss << HEALTH_SPRITE_PREFIX << game::avatar::Statistics::Read(game::avatar::Statistic::HEALTH);
		::visuals::Images::SetSprite(LAYOUT_NAME, HEALTH_IMAGE_ID, ss.str());
	}

	static void UpdateHunger(const Uint32&)
	{
		std::stringstream ss;
		ss << HUNGER_SPRITE_PREFIX << game::avatar::Statistics::Read(game::avatar::Statistic::HUNGER);
		::visuals::Images::SetSprite(LAYOUT_NAME, HUNGER_IMAGE_ID, ss.str());
	}

	const std::vector<::UIState> states =
	{
		::UIState::IN_PLAY_MAP,
		::UIState::IN_PLAY_INVENTORY,
		::UIState::IN_PLAY_STATUS,
		::UIState::IN_PLAY_COMBAT,
		::UIState::IN_PLAY_COMBAT_RESULT,
	};

	static void OnMouseMotion(const common::XY<Sint32>& position)
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, position);
		std::stringstream ss;
		if (areas.contains(AREA_HEALTH))
		{
			ss << "Health: " << game::avatar::Statistics::Read(game::avatar::Statistic::HEALTH);
		}
		else if(areas.contains(AREA_HUNGER))
		{
			ss << "Hunger: " << game::avatar::Statistics::Read(game::avatar::Statistic::HUNGER);
		}
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_TOOL_TOP, ss.str());
	}

	void Start()
	{
		for (auto state : states)
		{
			::application::MouseMotion::AddHandler(state, OnMouseMotion);
			::application::Update::AddHandler(state, UpdateHealth);
			::application::Update::AddHandler(state, UpdateHunger);
		}
	}
}