#include "Common.Data.Properties.h"
#include "Common.Utility.h"
#include "Common.XY.h"
#include "json.hpp"
#include "Graphics.Colors.h"
#include "Graphics.Data.Properties.h"
#include "Graphics.Sprites.h"
namespace graphics::Image
{
	static SDL_Color defaultColor = { 255, 255, 255, 255 };

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		if (model.count(data::Properties::VISIBLE) == 0 || model[data::Properties::VISIBLE] == true)
		{
			Sprites::Draw(
					model[data::Properties::SPRITE],
					renderer,
					common::XY<int>(
						(int)model[common::data::Properties::X],
						(int)model[common::data::Properties::Y]),
					(model.count(data::Properties::COLOR)>0) ?
					(::graphics::Colors::Read(model[data::Properties::COLOR])) :
					(defaultColor));
		}
	}
}
