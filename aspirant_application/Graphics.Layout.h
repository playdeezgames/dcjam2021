#pragma once
#include <memory>
#include <SDL.h>
#include "json.hpp"
namespace graphics::Layout
{
	void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&);
}
