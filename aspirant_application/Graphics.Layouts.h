#pragma once
#include <string>
#include <SDL.h>
#include <memory>
namespace graphics::Layouts
{
	void Draw(std::shared_ptr<SDL_Renderer>, const std::string&);
	void Start(const std::string&);
}