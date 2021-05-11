#pragma once
#include <SDL.h>
#include <memory>
namespace common::Application
{
	void Render(const std::shared_ptr<SDL_Renderer>& renderer);
}
