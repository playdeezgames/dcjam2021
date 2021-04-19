#include "Application.Renderer.h"
#include "Graphics.Layouts.h"
#include <map>
#include "Common.Audio.h"
#include "Options.h"
#include <sstream>
#include "Common.Utility.h"
#include "Application.Command.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.Menus.h"
#include "Graphics.MenuItems.h"
#include "Application.Sounds.h"
#include <SDL_mixer.h>
#include "Common.Utility.h"
namespace state::Options
{
	const std::string LAYOUT_NAME = "State.Options";
	const std::string MENU_ID = "Options";
	const std::string MUTE_MENU_ITEM_ID = "Mute";
	const std::string SFX_VOLUME_MENU_ITEM_ID = "SfxVolume";
	const std::string MUX_VOLUME_MENU_ITEM_ID = "MuxVolume";
	const std::string MUTE = "Mute";
	const std::string UNMUTE = "Unmute";
	const int VOLUME_DELTA = 8;

	enum class OptionsItem
	{
		TOGGLE_MUTE,
		SFX_VOLUME,
		MUX_VOLUME,
		BACK
	};

	static void AdjustSfxVolume(int delta)
	{
		common::audio::Sfx::SetVolume(common::audio::Sfx::GetVolume() + delta);
		common::audio::Sfx::Play(application::Sounds::WOOHOO);
		::Options::Save();
	}

	static void AdjustMuxVolume(int delta)
	{
		common::audio::Mux::SetVolume(common::audio::Mux::GetVolume() + delta);
		::Options::Save();
	}

	static OptionsItem GetCurrentItem()
	{
		return (OptionsItem)graphics::Menus::ReadValue(LAYOUT_NAME, MENU_ID).value();
	}

	const std::map<OptionsItem, std::function<void(int)>> itemChangers =
	{
		{ OptionsItem::SFX_VOLUME, AdjustSfxVolume },
		{ OptionsItem::MUX_VOLUME, AdjustMuxVolume }
	};

	static void ChangeItem(int delta)
	{
		auto iter = itemChangers.find(GetCurrentItem());
		if (iter != itemChangers.end())
		{
			iter->second(delta);
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

	const std::map<OptionsItem, std::function<void()>> activators =
	{
		{ OptionsItem::TOGGLE_MUTE, []() { common::Audio::SetMuted(!common::Audio::IsMuted()); ::Options::Save(); }},
		{ OptionsItem::BACK, []() { ::application::UIState::Write(::UIState::MAIN_MENU); }}
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, GetCurrentItem());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, [](){ graphics::Menus::Previous(LAYOUT_NAME, MENU_ID); }},
		{::Command::DOWN, []() { graphics::Menus::Next(LAYOUT_NAME, MENU_ID); }},
		{::Command::LEFT, DecreaseItem },
		{::Command::RIGHT, IncreaseItem },
		{::Command::BACK, []() { ::application::UIState::Write(::UIState::MAIN_MENU); }},
		{::Command::RED, []() { ::application::UIState::Write(::UIState::MAIN_MENU); }},
		{ ::Command::GREEN, ActivateItem }
	};

	static void OnCommand(const ::Command& command)
	{
		common::Utility::Dispatch(commandHandlers, command);
	}

	static void UpdateMuteMenuItem(const Uint32&)
	{
		graphics::MenuItems::SetText(LAYOUT_NAME, MUTE_MENU_ITEM_ID, 
			common::Audio::IsMuted() ? UNMUTE : MUTE);
	}

	static void UpdateSfxMenuItem(const Uint32&)
	{
		std::stringstream ss;
		ss << "SFX Volume (" << common::Utility::ToPercentage(common::audio::Sfx::GetVolume(), MIX_MAX_VOLUME) << "%)";
		graphics::MenuItems::SetText(LAYOUT_NAME, SFX_VOLUME_MENU_ITEM_ID, ss.str());
	}

	static void UpdateMuxMenuItem(const Uint32&)
	{
		std::stringstream ss;
		ss << "MUX Volume (" << common::Utility::ToPercentage(common::audio::Mux::GetVolume(), MIX_MAX_VOLUME) << "%)";
		graphics::MenuItems::SetText(LAYOUT_NAME, MUX_VOLUME_MENU_ITEM_ID, ss.str());
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::OPTIONS, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::OPTIONS, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::OPTIONS, UpdateMuteMenuItem);
		::application::Update::AddHandler(::UIState::OPTIONS, UpdateSfxMenuItem);
		::application::Update::AddHandler(::UIState::OPTIONS, UpdateMuxMenuItem);
	}
}