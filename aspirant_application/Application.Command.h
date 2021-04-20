#pragma once
#include "Command.h"
#include "UIState.h"
#include <functional>
#include <map>
namespace application::Command
{
	typedef std::function<void(const ::Command&)> Handler;
	void SetHandler(const ::UIState&, Handler);
	void SetHandlers(const ::UIState&, const std::map<::Command, std::function<void()>>&);
	void Handle(const ::Command& command);
}
