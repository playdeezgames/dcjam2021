#pragma once
#include <string>
#include <SDL.h>
#include "Common.XY.h"
#include <optional>
#include <memory>
namespace graphics::Sprites
{
	void Draw(const std::string&, std::shared_ptr<SDL_Renderer>, const ::common::XY<int>&, const SDL_Color&);
	std::optional<int> GetWidth(const std::string&);
}