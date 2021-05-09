#include "Application.Command.h"
#include "Application.Renderer.h"
#include <sstream>
#include "Game.Avatar.Statistics.h"
#include "Visuals.Texts.h"
#include "Visuals.Areas.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Game.Avatar.h"
#include "Common.Audio.h"
#include "Application.OnEnter.h"
namespace state::in_play::AvatarStatus
{
	const std::string LAYOUT_NAME = "State.InPlay.AvatarStatus";
	const std::string TEXT_ATTACK = "Attack";
	const std::string TEXT_ATTACK_TIMER = "AttackTimer";
	const std::string TEXT_DEFEND = "Defend";
	const std::string TEXT_DEFEND_TIMER = "DefendTimer";
	const std::string TEXT_BOWEL = "Bowel";
	const std::string TEXT_DRUNKENNESS = "Drunkenness";
	const std::string TEXT_NAUSEA = "Nausea";	
	const std::string TEXT_KEYS= "Keys";
	const std::string TEXT_POOP = "Poop";
	const std::string CAN_POOP = "[Poop!]";
	const std::string CANNOT_POOP = "";
	const std::string AREA_POOP = "Poop";


	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, []() {application::UIState::Write(::UIState::LEAVE_PLAY); }},
		{ ::Command::PREVIOUS, []() {application::UIState::Write(::UIState::IN_PLAY_INVENTORY); }},
		{ ::Command::NEXT, []() {application::UIState::Write(::UIState::IN_PLAY_MAP); }},
		{ ::Command::YELLOW, []() {application::UIState::Write(::UIState::IN_PLAY_MAP); }}
	};

	static void UpdateAttack()
	{
		std::stringstream ss;
		ss << "Courage: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_ATTACK, ss.str());
	}

	static void UpdateAttackTimer()
	{
		auto value = game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK_TIMER);
		std::stringstream ss;
		if (value > 0)
		{
			ss << "Duration: ";
			ss << value;
		}
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_ATTACK_TIMER, ss.str());
	}

	static void UpdateDefend()
	{
		std::stringstream ss;
		ss << "Alertness: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_DEFEND, ss.str());
	}

	static void UpdateDefendTimer()
	{
		auto value = game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND_TIMER);
		std::stringstream ss;
		if (value > 0)
		{
			ss << "Duration: ";
			ss << value;
		}
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_DEFEND_TIMER, ss.str());
	}

	static void UpdateBowel()
	{
		std::stringstream ss;
		ss << "Bowel: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::BOWEL);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_BOWEL, ss.str());
	}

	static void UpdateDrunkenness()
	{
		std::stringstream ss;
		ss << "Drunkenness: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::DRUNKENNESS);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_DRUNKENNESS, ss.str());
	}

	static void UpdateNausea()
	{
		std::stringstream ss;
		ss << "Nausea: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::NAUSEA);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_NAUSEA, ss.str());
	}

	static void UpdateKeys()
	{
		std::stringstream ss;
		ss << "Keys: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::KEYS);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_KEYS, ss.str());
	}

	static void UpdatePoop()
	{
		if (!game::Avatar::CanPoop())
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_POOP, CANNOT_POOP);
		}
		else
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_POOP, CAN_POOP);
		}
	}

	static void OnMouseMotionInArea(const std::string& area)
	{
		if (area == AREA_POOP)
		{
			visuals::Texts::SetColor(LAYOUT_NAME, TEXT_POOP, "Cyan");//TODO: magic string
		}
		else
		{
			visuals::Texts::SetColor(LAYOUT_NAME, TEXT_POOP, "Gray");//TODO: magic string
		}
	}

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		if (area == AREA_POOP && game::Avatar::CanPoop())
		{
			common::audio::Sfx::Play(game::Avatar::Poop(false));
			application::OnEnter::Handle();
			return true;
		}
		return false;
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_STATUS, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_STATUS, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea));

		::application::Command::SetHandlers(::UIState::IN_PLAY_STATUS, commandHandlers);

		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_STATUS, LAYOUT_NAME);

		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateAttack);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateAttackTimer);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateDefend);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateDefendTimer);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateBowel);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateDrunkenness);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateNausea);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdateKeys);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATUS, UpdatePoop);
	}
}