#include "Common.Utility.h"
#include "Graphics.Fonts.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
#include "json.hpp"
namespace graphics::Text
{
	void Draw(SDL_Renderer* renderer, const nlohmann::json& model)
	{
		if ((bool)model[graphics::Properties::DROP_SHADOW])
		{
			graphics::Fonts::Get(
				model[graphics::Properties::FONT])
				.value()
				.WriteText(
					renderer, 
					common::XY<int>(
						(int)model[common::Properties::X] + (int)model[graphics::Properties::DROP_SHADOW_X], 
						(int)model[common::Properties::Y] + (int)model[graphics::Properties::DROP_SHADOW_Y]),
					model[graphics::Properties::TEXT],
					model[graphics::Properties::DROP_SHADOW_COLOR],
					(HorizontalAlignment)(int)model[graphics::Properties::HORIZONTAL_ALIGNMENT]);
		}
		graphics::Fonts::Get(
			model[graphics::Properties::FONT])
			.value()
			.WriteText(
				renderer, 
				common::XY<int>(model[common::Properties::X], model[common::Properties::Y]),
				model[graphics::Properties::TEXT],
				model[graphics::Properties::COLOR],
				(HorizontalAlignment)(int)model[graphics::Properties::HORIZONTAL_ALIGNMENT]);
	}
}
