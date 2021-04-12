#pragma once
#include <string>
#include <optional>
#include <SDL.h>
#include "Common.XY.h"
#include "json.hpp"
namespace graphics::Sprites
{
	void InitializeFromFile(const std::string&);
	void Draw(const std::string&, std::shared_ptr<SDL_Renderer>, const ::common::XY<int>&, const SDL_Color&);
	void Draw(const std::string&, std::shared_ptr<SDL_Renderer>, const ::common::XY<int>&);
	//TODO: make me optional
	int GetWidth(const std::string&);

}