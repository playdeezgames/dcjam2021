#pragma once
#include "UIState.h"
#include <string>
#include <SDL.h>
namespace application::Renderer
{
	void SetRenderLayout(const ::UIState&, const std::string&);
	void Render(SDL_Renderer* renderer);
}
