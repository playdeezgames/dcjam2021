#include "Common.Data.Properties.h"
#include "Common.Utility.h"
#include "Common.XY.h"
#include "json.hpp"
#include "Graphics.Colors.h"
#include "Graphics.Properties.h"
#include "Graphics.Sprites.h"
namespace graphics::Image
{
	static SDL_Color defaultColor = { 255, 255, 255, 255 };

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		if (model.count(Properties::VISIBLE) == 0 || model[Properties::VISIBLE] == true)
		{
			Sprites::Draw(
					model[Properties::SPRITE],
					renderer,
					common::XY<int>(
						(int)model[common::data::Properties::X],
						(int)model[common::data::Properties::Y]),
					(model.count(Properties::COLOR)>0) ?
					(::graphics::Colors::Read(model[Properties::COLOR])) :
					(defaultColor));
		}
	}
}
