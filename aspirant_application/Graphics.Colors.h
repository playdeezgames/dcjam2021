#pragma once
#include <string>
#include <SDL.h>
#include <vector>
namespace graphics::Colors
{
	void InitializeFromFile(const std::string&);
	SDL_Color Read(const std::string&);
}