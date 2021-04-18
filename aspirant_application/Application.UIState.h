#pragma once
#include "UIState.h"
#include <optional>
#include <string>
namespace application::UIState
{
	void Write(const ::UIState&);
	const ::UIState& Read();
	std::optional<std::string> EnterGame();
}