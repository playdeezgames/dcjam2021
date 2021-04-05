#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include <sstream>
#include "Game.Avatar.Statistics.h"
#include "Graphics.Texts.h"
namespace state::in_play::AvatarStatus
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarStatus";
	const std::string ATTACK_TEXT_ID = "Attack";
	const std::string ATTACK_TIMER_TEXT_ID = "AttackTimer";
	const std::string DEFEND_TEXT_ID = "Defend";
	const std::string DEFEND_TIMER_TEXT_ID = "DefendTimer";

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
			application::UIState::Write(::UIState::LEAVE_PLAY);
			break;
		case ::Command::PREVIOUS:
			application::UIState::Write(::UIState::IN_PLAY_INVENTORY);
			break;
		case ::Command::NEXT:
		case ::Command::YELLOW:
			application::UIState::Write(::UIState::IN_PLAY_MAP);
			break;
		}
	}

	static void UpdateAttack()
	{
		std::stringstream ss;
		ss << "Attack: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK);
		graphics::Texts::SetText(LAYOUT_NAME, ATTACK_TEXT_ID, ss.str());
	}

	static void UpdateAttackTimer()
	{
		std::stringstream ss;
		ss << "Duration: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK_TIMER);
		graphics::Texts::SetText(LAYOUT_NAME, ATTACK_TIMER_TEXT_ID, ss.str());
	}

	static void UpdateDefend()
	{
		std::stringstream ss;
		ss << "Defend: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND);
		graphics::Texts::SetText(LAYOUT_NAME, DEFEND_TEXT_ID, ss.str());
	}

	static void UpdateDefendTimer()
	{
		std::stringstream ss;
		ss << "Duration: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND_TIMER);
		graphics::Texts::SetText(LAYOUT_NAME, DEFEND_TIMER_TEXT_ID, ss.str());
	}


	static void OnUpdate(const Uint32& ticks)
	{
		UpdateAttack();
		UpdateAttackTimer();
		UpdateDefend();
		UpdateDefendTimer();
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_STATUS, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_STATUS, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, OnUpdate);
	}
}