#pragma once
#include <string>
#include "UIState.h"
#include <SDL.h>
#include <functional>
namespace application::TextInput
{
	typedef std::function<void(const std::string&)> Handler;
	void SetHandler(const ::UIState&, Handler);
	void Handle(const SDL_TextInputEvent&);
}
