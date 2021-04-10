#include "json.hpp"
#include <SDL.h>
#include "Graphics.Layouts.h"
#include "Common.Properties.h"
namespace graphics::Sublayout
{
	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		graphics::Layouts::Draw(renderer, model[common::Properties::NAME]);
	}
}