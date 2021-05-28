#pragma once
#include <string>
#include "Application.Command.h"
#include <optional>
namespace application::Keyboard
{
	std::optional<::Command> ToCommand(int);
}
