#pragma once
#include <SDL.h>
#include <map>
#include <string>
#include <memory>
namespace visuals::Textures
{
	const std::shared_ptr<SDL_Texture>& Read(const std::shared_ptr<SDL_Renderer>&, const std::string&);
}

