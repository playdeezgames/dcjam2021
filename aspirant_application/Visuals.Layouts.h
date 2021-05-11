#pragma once
#include <string>
#include <SDL.h>
#include <memory>
#include "json.hpp"
#include <vector>
namespace visuals::Layouts
{
	void Draw(const std::shared_ptr<SDL_Renderer>&, const std::string&);
	void Start();
}