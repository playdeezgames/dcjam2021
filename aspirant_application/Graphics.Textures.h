#pragma once
#include <SDL.h>
#include <map>
#include <string>
#include <memory>
namespace graphics::Textures
{
	void InitializeFromFile(std::shared_ptr<SDL_Renderer>, const std::string&);
	SDL_Texture* Read(const std::string&);
}

