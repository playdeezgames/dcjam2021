#pragma once
#include <SDL.h>
#include <map>
#include <string>
#include <memory>
namespace visuals::Textures
{
	std::shared_ptr<SDL_Texture> Read(std::shared_ptr<SDL_Renderer>, const std::string&);
}

