#pragma once
#include "Application.UIState.h"
#include <optional>
#include <string>
#include <functional>
enum class UIState
{
	SPLASH,
	MAIN_MENU,
	CONFIRM_QUIT,
	QUIT,
	ABOUT,
	OPTIONS,
	START_GAME,
	IN_PLAY_MAP,
	IN_PLAY_FLOOR,
	IN_PLAY_INVENTORY,
	IN_PLAY_STATUS,
	IN_PLAY_COMBAT,
	IN_PLAY_COMBAT_RESULT,
	IN_PLAY_DEAD,
	LEAVE_PLAY,
	IN_PLAY_EXIT,
	LOAD_GAME,
	SAVE_GAME,
	CONFIRM_ABANDON,
	CONFIRM_OVERWRITE_SLOT1,
	CONFIRM_OVERWRITE_SLOT2,
	CONFIRM_OVERWRITE_SLOT3,
	CONFIRM_OVERWRITE_SLOT4,
	CONFIRM_OVERWRITE_SLOT5,
	IN_PLAY_TRADE
};
namespace application::UIState
{
	void Write(const ::UIState&);
	const ::UIState& Read();
	std::optional<std::string> EnterGame();
	std::function<void()> GoTo(::UIState);
}