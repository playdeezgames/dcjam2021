#pragma once
#include "Command.h"
#include "UIState.h"
namespace application::Command
{
	typedef void (*Handler)(const ::Command&);
	void SetHandler(const ::UIState&, Handler);
	void Handle(const ::Command& command);
}
