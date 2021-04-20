#pragma once
#include <string>
#include <SDL.h>
#include "Application.Command.h"
#include <optional>
namespace application::Keyboard
{
	std::optional<::Command> ToCommand(SDL_KeyCode);
}
