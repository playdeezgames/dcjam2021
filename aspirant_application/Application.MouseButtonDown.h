#pragma once
#include <SDL.h>
#include "Common.XY.h"
#include "Application.UIState.h"
#include <functional>
namespace application::MouseButtonDown
{
	typedef std::function<void(const common::XY<Sint32>&, Uint8)> Handler;
	void SetHandler(const ::UIState&, Handler);
	void Handle(const SDL_MouseButtonEvent&);
}
