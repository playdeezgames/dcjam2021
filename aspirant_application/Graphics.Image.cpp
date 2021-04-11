#include "Common.Properties.h"
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
			Sprites::Read(model[Properties::SPRITE])
				.value()
				.Draw(
					renderer,
					common::XY<int>(
						(int)model[common::Properties::X],
						(int)model[common::Properties::Y]),
					(model.count(Properties::COLOR)>0) ?
					(::graphics::Colors::Read(model[Properties::COLOR])) :
					(defaultColor));
		}
	}
}
