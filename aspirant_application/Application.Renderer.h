#pragma once
#include "UIState.h"
#include <string>
#include <SDL.h>
namespace application::Renderer
{
	void SetRenderLayout(const ::UIState&, const std::string&);
}
namespace common::Application
{
	void Render(SDL_Renderer* renderer);
}
