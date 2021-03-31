#include "Application.Renderer.h"
#include "Graphics.Layouts.h"
#include <map>
#include "Common.Sounds.h"
#include "Options.h"
#include <sstream>
#include "Common.Utility.h"
#include "Application.Command.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.Menus.h"
#include "Graphics.MenuItems.h"
namespace state::Options
{
	const std::string LAYOUT_NAME = "State.Options";
	const std::string MENU_ID = "Options";
	const std::string MUTE_MENU_ITEM_ID = "Mute";
	const std::string SFX_VOLUME_MENU_ITEM_ID = "SfxVolume";
	const std::string MUX_VOLUME_MENU_ITEM_ID = "MuxVolume";
	const std::string MUTE = "Mute";
	const std::string UNMUTE = "Unmute";
	const std::string SFX_SAMPLE_NAME = "woohoo";

	enum class OptionsItem
	{
		TOGGLE_MUTE,
		SFX_VOLUME,
		MUX_VOLUME,
		BACK
	};

	const int VOLUME_DELTA = 8;

	static void AdjustSfxVolume(int delta)
	{
		common::Sounds::SetSfxVolume(common::Sounds::GetSfxVolume() + delta);
		common::Sounds::PlaySound(SFX_SAMPLE_NAME);
		
		::Options::Save();
	}

	static void AdjustMuxVolume(int delta)
	{
		common::Sounds::SetMuxVolume(common::Sounds::GetMuxVolume() + delta);
		::Options::Save();
	}

	static OptionsItem GetCurrentItem()
	{
		return (OptionsItem)graphics::Menus::Read(LAYOUT_NAME, MENU_ID).value();
	}

	static void ChangeItem(int delta)
	{
		switch (GetCurrentItem())
		{
		case OptionsItem::SFX_VOLUME:
			AdjustSfxVolume(delta);
			break;
		case OptionsItem::MUX_VOLUME:
			AdjustMuxVolume(delta);
			break;
		}
	}

	static void IncreaseItem()
	{
		ChangeItem(VOLUME_DELTA);
	}

	static void DecreaseItem()
	{
		ChangeItem(-VOLUME_DELTA);
	}

	static void ActivateItem()
	{
		switch (GetCurrentItem())
		{
		case OptionsItem::TOGGLE_MUTE:
			common::Sounds::SetMuted(!common::Sounds::IsMuted());
			::Options::Save();
			break;
		case OptionsItem::BACK:
			::application::UIState::Write(::UIState::MAIN_MENU);
			break;
		}
	}

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::UP:
			graphics::Menus::Previous(LAYOUT_NAME, MENU_ID);
			break;
		case ::Command::DOWN:
			graphics::Menus::Next(LAYOUT_NAME, MENU_ID);
			break;
		case ::Command::LEFT:
			DecreaseItem();
			break;
		case ::Command::RIGHT:
			IncreaseItem();
			break;
		case ::Command::BACK:
		case ::Command::RED:
			::application::UIState::Write(::UIState::MAIN_MENU);
			break;
		case ::Command::GREEN:
			ActivateItem();
			break;
		}
	}

	static void UpdateMuteMenuItem()
	{
		graphics::MenuItems::SetItemText(LAYOUT_NAME, MUTE_MENU_ITEM_ID, 
			common::Sounds::IsMuted() ? UNMUTE : MUTE);
	}

	static void UpdateSfxMenuItem()
	{
		std::stringstream ss;
		ss << "SFX Volume (" << common::Utility::ToPercentage(common::Sounds::GetSfxVolume(), MIX_MAX_VOLUME) << "%)";
		graphics::MenuItems::SetItemText(LAYOUT_NAME, SFX_VOLUME_MENU_ITEM_ID, ss.str());
	}

	static void UpdateMuxMenuItem()
	{
		std::stringstream ss;
		ss << "MUX Volume (" << common::Utility::ToPercentage(common::Sounds::GetMuxVolume(), MIX_MAX_VOLUME) << "%)";
		graphics::MenuItems::SetItemText(LAYOUT_NAME, MUX_VOLUME_MENU_ITEM_ID, ss.str());
	}

	static void OnUpdate(const Uint32& ticks)
	{
		UpdateMuteMenuItem();
		UpdateSfxMenuItem();
		UpdateMuxMenuItem();
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::OPTIONS, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::OPTIONS, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::OPTIONS, OnUpdate);
	}
}