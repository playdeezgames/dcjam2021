#include <string>
#include <sstream>
#include "Application.UIState.h"
#include "Application.Update.h"
#include "Graphics.Texts.h"
#include "Game.Avatar.h"
#include "Game.Creatures.h"
namespace sublayout::EnemyStats
{
	const std::string LAYOUT_NAME = "Sublayout.EnemyStats";
	const std::string HEALTH_TEXT_ID = "EnemyHealth";

	static void UpdateHealth(const Uint32&)
	{
		std::stringstream ss;
		ss << "Enemy: ";
		auto position = game::Avatar::GetPosition();
		ss << game::Creatures::GetHealth(position).value() << "/";
		ss << game::Creatures::GetMaximumHealth(position).value();
		
		::graphics::Texts::SetText(LAYOUT_NAME, HEALTH_TEXT_ID, ss.str());
	}

	void Start()
	{
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT, UpdateHealth);
		::application::Update::AddHandler(::UIState::IN_PLAY_COMBAT_RESULT, UpdateHealth);
	}
}