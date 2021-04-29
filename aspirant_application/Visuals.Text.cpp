#include "Common.Utility.h"
#include "Visuals.Fonts.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "json.hpp"
namespace graphics::Text
{
	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		if ((bool)model[graphics::data::Properties::DROP_SHADOW])
		{
			graphics::Fonts::WriteText(
					model[graphics::data::Properties::FONT],
					renderer, 
					common::XY<int>
					(
						(int)model[common::data::Properties::X] + (int)model[graphics::data::Properties::DROP_SHADOW_X],
						(int)model[common::data::Properties::Y] + (int)model[graphics::data::Properties::DROP_SHADOW_Y]
					),
					model[graphics::data::Properties::TEXT],
					model[graphics::data::Properties::DROP_SHADOW_COLOR],
					(HorizontalAlignment)(int)model[graphics::data::Properties::HORIZONTAL_ALIGNMENT]);
		}
		graphics::Fonts::WriteText(
				model[graphics::data::Properties::FONT],
				renderer, 
				common::XY<int>
				(
					model[common::data::Properties::X],
					model[common::data::Properties::Y]
				),
				model[graphics::data::Properties::TEXT],
				model[graphics::data::Properties::COLOR],
				(HorizontalAlignment)(int)model[graphics::data::Properties::HORIZONTAL_ALIGNMENT]);
	}
}
