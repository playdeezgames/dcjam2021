#pragma once
#include <string>
#include <SDL.h>
#include <memory>
#include "json.hpp"
namespace graphics::Layouts
{
	void Draw(std::shared_ptr<SDL_Renderer>, const std::string&);
	//nlohmann::json& GetLayout(const std::string&);//TODO: DAL?
}