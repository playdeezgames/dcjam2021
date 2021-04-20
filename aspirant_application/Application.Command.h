#pragma once
#include "Command.h"
#include "UIState.h"
#include <functional>
#include <map>
namespace application::Command
{
	void SetHandlers(const ::UIState&, const std::map<::Command, std::function<void()>>&);
	void Handle(const ::Command& command);
}
