#include "Common.Utility.h"
#include "Graphics.Fonts.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
#include "json.hpp"
namespace graphics::Text
{
	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		if ((bool)model[graphics::Properties::DROP_SHADOW])
		{
			graphics::Fonts::WriteText(
					model[graphics::Properties::FONT],
					renderer, 
					common::XY<int>
					(
						(int)model[common::data::Properties::X] + (int)model[graphics::Properties::DROP_SHADOW_X],
						(int)model[common::data::Properties::Y] + (int)model[graphics::Properties::DROP_SHADOW_Y]
					),
					model[graphics::Properties::TEXT],
					model[graphics::Properties::DROP_SHADOW_COLOR],
					(HorizontalAlignment)(int)model[graphics::Properties::HORIZONTAL_ALIGNMENT]);
		}
		graphics::Fonts::WriteText(
				model[graphics::Properties::FONT],
				renderer, 
				common::XY<int>
				(
					model[common::data::Properties::X],
					model[common::data::Properties::Y]
				),
				model[graphics::Properties::TEXT],
				model[graphics::Properties::COLOR],
				(HorizontalAlignment)(int)model[graphics::Properties::HORIZONTAL_ALIGNMENT]);
	}
}
