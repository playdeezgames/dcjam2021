#include "json.hpp"
#include <SDL.h>
#include "Visuals.Layouts.h"
#include "Common.Data.Properties.h"
namespace visuals::Sublayout
{
	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		visuals::Layouts::Draw(renderer, model[common::data::Properties::NAME]);
	}
}