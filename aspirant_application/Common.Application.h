#pragma once
#include <SDL.h>
#include <memory>
namespace common::Application
{
	void Render(std::shared_ptr<SDL_Renderer> renderer);
}
