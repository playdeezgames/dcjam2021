#pragma once
#include <string>
#include <SDL.h>
#include "Command.h"
#include <optional>
namespace application::Keyboard
{
	void InitializeFromFile(const std::string&);
	std::optional<::Command> ToCommand(SDL_KeyCode);
}
