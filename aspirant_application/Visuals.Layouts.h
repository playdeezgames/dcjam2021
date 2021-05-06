#pragma once
#include <string>
#include <SDL.h>
#include <memory>
#include "json.hpp"
#include <vector>
namespace visuals::Layouts
{
	void InitializeLayout(const std::string&);
	void Draw(std::shared_ptr<SDL_Renderer>, const std::string&);
}