#pragma once
#include <SDL.h>
#include "Common.XY.h"
#include "Application.UIState.h"
#include <functional>
namespace application::MouseMotion
{
	typedef std::function<void(const common::XY<Sint32>&)> Handler;
	void AddHandler(const ::UIState&, Handler);
	void Handle(const SDL_MouseMotionEvent&);
}
