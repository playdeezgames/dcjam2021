#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include <sstream>
#include "Game.Avatar.Statistics.h"
#include "Graphics.Texts.h"
#include "Common.Utility.h"
namespace state::in_play::AvatarStatus
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarStatus";
	const std::string ATTACK_TEXT_ID = "Attack";
	const std::string ATTACK_TIMER_TEXT_ID = "AttackTimer";
	const std::string DEFEND_TEXT_ID = "Defend";
	const std::string DEFEND_TIMER_TEXT_ID = "DefendTimer";

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, []() {application::UIState::Write(::UIState::LEAVE_PLAY); }},
		{ ::Command::PREVIOUS, []() {application::UIState::Write(::UIState::IN_PLAY_INVENTORY); }},
		{ ::Command::NEXT, []() {application::UIState::Write(::UIState::IN_PLAY_MAP); }},
		{ ::Command::YELLOW, []() {application::UIState::Write(::UIState::IN_PLAY_MAP); }}
	};

	static void OnCommand(const ::Command& command)
	{
		common::Utility::Dispatch(commandHandlers, command);
	}

	static void UpdateAttack(const Uint32&)
	{
		std::stringstream ss;
		ss << "Attack: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK);
		graphics::Texts::SetText(LAYOUT_NAME, ATTACK_TEXT_ID, ss.str());
	}

	static void UpdateAttackTimer(const Uint32&)
	{
		auto value = game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK_TIMER);
		std::stringstream ss;
		if (value > 0)
		{
			ss << "Duration: ";
			ss << value;
		}
		graphics::Texts::SetText(LAYOUT_NAME, ATTACK_TIMER_TEXT_ID, ss.str());
	}

	static void UpdateDefend(const Uint32&)
	{
		std::stringstream ss;
		ss << "Defend: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND);
		graphics::Texts::SetText(LAYOUT_NAME, DEFEND_TEXT_ID, ss.str());
	}

	static void UpdateDefendTimer(const Uint32&)
	{
		auto value = game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND_TIMER);
		std::stringstream ss;
		if (value > 0)
		{
			ss << "Duration: ";
			ss << value;
		}
		graphics::Texts::SetText(LAYOUT_NAME, DEFEND_TIMER_TEXT_ID, ss.str());
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY_STATUS, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_STATUS, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateAttack);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateAttackTimer);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateDefend);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateDefendTimer);
	}
}