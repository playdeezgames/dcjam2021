#pragma once
#include <SDL.h>
#include <map>
#include <string>
#include <memory>
namespace graphics::Textures
{
	void InitializeFromFile(std::shared_ptr<SDL_Renderer>, const std::string&);//TODO: DAL
	std::shared_ptr<SDL_Texture> Read(const std::string&);
}

