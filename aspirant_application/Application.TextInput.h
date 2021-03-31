#pragma once
#include <string>
#include "UIState.h"
#include <SDL.h>
namespace application::TextInput
{
	typedef void (*Handler)(const std::string&);
	void SetHandler(const UIState&, Handler);
	void Handle(const SDL_TextInputEvent&);
}
