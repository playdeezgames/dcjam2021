#pragma once
#include <string>
#include <SDL.h>
namespace graphics::Layouts
{
	void Draw(SDL_Renderer*, const std::string&);
	void Start(const std::string&);
}