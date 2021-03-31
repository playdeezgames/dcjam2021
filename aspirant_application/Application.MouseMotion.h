#pragma once
#include <SDL.h>
#include "Common.XY.h"
#include "UIState.h"
namespace application::MouseMotion
{
	typedef void (*Handler)(const common::XY<Sint32>&);
	void SetHandler(const UIState&, Handler);
	void Handle(const SDL_MouseMotionEvent&);
}
