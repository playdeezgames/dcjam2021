#pragma once
#include "Command.h"
#include "UIState.h"
#include <functional>
namespace application::Command
{
	typedef std::function<void(const ::Command&)> Handler;
	void SetHandler(const ::UIState&, Handler);
	void Handle(const ::Command& command);
}
