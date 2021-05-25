#include "Application.Renderer.h"
#include "Common.Audio.h"
#include "Options.h"
#include <sstream>
#include "Application.Command.h"
#include "Application.OnEnter.h"
#include "Visuals.Menus.h"
#include "Application.Sounds.h"
#include "Common.Utility.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
#include <SDL_mixer.h>
namespace state::Options
{
	const std::string LAYOUT_NAME = "State.Options";
	const std::string MENU_ID = "Options";
	const std::string MENU_ITEM_MUTE = "Mute";
	const std::string MENU_ITEM_SFX_VOLUME = "SfxVolume";
	const std::string MENU_ITEM_MUX_VOLUME = "MuxVolume";
	const std::string MUTE = "Mute";
	const std::string UNMUTE = "Unmute";
	const std::string AREA_MUTE = "Mute";
	const std::string AREA_BACK = "Back";
	const std::string AREA_SFX_DECREASE = "DecreaseSfxVolume";
	const std::string AREA_SFX_INCREASE = "IncreaseSfxVolume";
	const std::string AREA_MUX_DECREASE = "DecreaseMuxVolume";
	const std::string AREA_MUX_INCREASE = "IncreaseMuxVolume";
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
		common::audio::Sfx::Play(application::Sounds::Read(application::UI::Sfx::WOOHOO));
		::Options::Save();
	}

	static void AdjustMuxVolume(int delta)
	{
		common::audio::Mux::SetVolume(common::audio::Mux::GetVolume() + delta);
		::Options::Save();
	}

	static OptionsItem GetCurrentItem()
	{
		return (OptionsItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value();
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
		application::OnEnter::Handle();
	}

	static void DecreaseItem()
	{
		ChangeItem(-VOLUME_DELTA);
		application::OnEnter::Handle();
	}

	static void ToggleMute()
	{
		common::Audio::SetMuted(!common::Audio::IsMuted()); 
		::Options::Save(); 
		application::OnEnter::Handle();
	}

	const std::map<OptionsItem, std::function<void()>> activators =
	{
		{ OptionsItem::TOGGLE_MUTE, ToggleMute },
		{ OptionsItem::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, GetCurrentItem());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::LEFT, DecreaseItem },
		{::Command::RIGHT, IncreaseItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::GREEN, ActivateItem }
	};

	static void OnCommand(const ::Command& command)
	{
		common::Utility::Dispatch(commandHandlers, command);
	}

	static void UpdateMuteMenuItem()
	{
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_MUTE, 
			common::Audio::IsMuted() ? UNMUTE : MUTE);
	}

	static void UpdateSfxMenuItem()
	{
		std::stringstream ss;
		ss << "< SFX Volume (" << common::Utility::ToPercentage(common::audio::Sfx::GetVolume(), MIX_MAX_VOLUME) << "%) >";
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_SFX_VOLUME, ss.str());
	}

	static void UpdateMuxMenuItem()
	{
		std::stringstream ss;
		ss << "< MUX Volume (" << common::Utility::ToPercentage(common::audio::Mux::GetVolume(), MIX_MAX_VOLUME) << "%) >";
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_MUX_VOLUME, ss.str());
	}

	static void SetCurrentMenuItem(OptionsItem item)
	{
		visuals::Menus::WriteIndex(LAYOUT_NAME, MENU_ID, (int)item);
	}

	const std::map<std::string, OptionsItem> areaMenuItems =
	{
		{ AREA_MUTE, OptionsItem::TOGGLE_MUTE },
		{ AREA_MUX_DECREASE, OptionsItem::MUX_VOLUME },
		{ AREA_MUX_INCREASE, OptionsItem::MUX_VOLUME },
		{ AREA_SFX_DECREASE, OptionsItem::SFX_VOLUME },
		{ AREA_SFX_INCREASE, OptionsItem::SFX_VOLUME },
		{ AREA_BACK, OptionsItem::BACK }
	};

	static void OnMouseMotionInArea(const std::string& area, const common::XY<Sint32>&)
	{
		SetCurrentMenuItem(areaMenuItems.find(area)->second);
	}

	const std::map<std::string, std::function<void()>> areaClickActions =
	{
		{AREA_MUX_DECREASE, DecreaseItem},
		{AREA_SFX_DECREASE, DecreaseItem},
		{AREA_MUX_INCREASE, IncreaseItem},
		{AREA_SFX_INCREASE, IncreaseItem}
	};

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		auto iter = areaClickActions.find(area);
		if (iter != areaClickActions.end())
		{
			iter->second();
			return true;
		}
		ActivateItem();
		return true;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::OPTIONS, common::audio::Mux::GoToTheme(common::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::OPTIONS, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(::UIState::OPTIONS, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea));
		::application::Command::SetHandlers(::UIState::OPTIONS, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::OPTIONS, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::OPTIONS, UpdateMuteMenuItem);
		::application::OnEnter::AddHandler(::UIState::OPTIONS, UpdateSfxMenuItem);
		::application::OnEnter::AddHandler(::UIState::OPTIONS, UpdateMuxMenuItem);
	}
}