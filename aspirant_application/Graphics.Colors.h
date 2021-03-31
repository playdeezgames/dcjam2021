#pragma once
#include <string>
#include <SDL.h>
#include <vector>
namespace graphics::Colors
{
	void Start(const std::string&);
	SDL_Color Get(const std::string&);
}