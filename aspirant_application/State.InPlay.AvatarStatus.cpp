#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include <sstream>
#include "Game.Avatar.Statistics.h"
#include "Visuals.Texts.h"
#include "Common.Utility.h"
#include "Visuals.Areas.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Data.Stores.h"
#include "Game.Data.Properties.h"
#include "Game.Avatar.h"
#include "Common.Audio.h"
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

	static void UpdateAttack(const Uint32&)
	{
		std::stringstream ss;
		ss << "Courage: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_ATTACK, ss.str());
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
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_ATTACK_TIMER, ss.str());
	}

	static void UpdateDefend(const Uint32&)
	{
		std::stringstream ss;
		ss << "Alertness: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_DEFEND, ss.str());
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
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_DEFEND_TIMER, ss.str());
	}

	static void UpdateBowel(const Uint32&)
	{
		std::stringstream ss;
		ss << "Bowel: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::BOWEL);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_BOWEL, ss.str());
	}

	static void UpdateDrunkenness(const Uint32&)
	{
		std::stringstream ss;
		ss << "Drunkenness: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::DRUNKENNESS);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_DRUNKENNESS, ss.str());
	}

	static void UpdateNausea(const Uint32&)
	{
		std::stringstream ss;
		ss << "Nausea: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::NAUSEA);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_NAUSEA, ss.str());
	}

	static void UpdateKeys(const Uint32&)
	{
		std::stringstream ss;
		ss << "Keys: ";
		ss << game::avatar::Statistics::Read(game::avatar::Statistic::KEYS);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_KEYS, ss.str());
	}

	static void UpdatePoop(const Uint32&)
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

	static void OnMouseMotion(const common::XY<Sint32>& xy)
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		if (areas.contains(AREA_POOP))
		{
			visuals::Texts::SetColor(LAYOUT_NAME, TEXT_POOP, "Cyan");//TODO: magic string
		}
		else
		{
			visuals::Texts::SetColor(LAYOUT_NAME, TEXT_POOP, "Gray");//TODO: magic string
		}

	}

	static bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8 buttons)
	{
		auto areas = visuals::Areas::Get(LAYOUT_NAME, xy);
		if (areas.contains(AREA_POOP) && game::Avatar::CanPoop())
		{
			common::audio::Sfx::Play(game::Avatar::Poop());
			return true;
		}
		return false;
	}

	void Start()
	{
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_STATUS, OnMouseButtonUp);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_STATUS, OnMouseMotion);
		::application::Command::SetHandlers(::UIState::IN_PLAY_STATUS, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_STATUS, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateAttack);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateAttackTimer);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateDefend);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateDefendTimer);

		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateBowel);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateDrunkenness);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateNausea);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdateKeys);
		::application::Update::AddHandler(::UIState::IN_PLAY_STATUS, UpdatePoop);

	}
}