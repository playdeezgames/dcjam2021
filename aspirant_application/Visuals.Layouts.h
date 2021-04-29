#pragma once
#include <string>
#include <SDL.h>
#include <memory>
#include "json.hpp"
#include <vector>
namespace graphics::Layouts
{
	void Draw(std::shared_ptr<SDL_Renderer>, const std::string&);
}