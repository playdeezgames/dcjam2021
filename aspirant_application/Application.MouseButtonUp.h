#pragma once
#include <SDL.h>
#include "Common.XY.h"
#include "Application.UIState.h"
#include <functional>
namespace application::MouseButtonUp
{
	typedef std::function<bool(const common::XY<Sint32>&, Uint8)> Handler;
	void AddHandler(const ::UIState&, Handler);
	void Handle(const SDL_MouseButtonEvent&);
}
