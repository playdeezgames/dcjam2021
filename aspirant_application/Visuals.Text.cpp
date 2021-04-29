#include "Common.Utility.h"
#include "Visuals.Fonts.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "json.hpp"
namespace visuals::Text
{
	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		if ((bool)model[visuals::data::Properties::DROP_SHADOW])
		{
			visuals::Fonts::WriteText(
					model[visuals::data::Properties::FONT],
					renderer, 
					common::XY<int>
					(
						(int)model[common::data::Properties::X] + (int)model[visuals::data::Properties::DROP_SHADOW_X],
						(int)model[common::data::Properties::Y] + (int)model[visuals::data::Properties::DROP_SHADOW_Y]
					),
					model[visuals::data::Properties::TEXT],
					model[visuals::data::Properties::DROP_SHADOW_COLOR],
					(HorizontalAlignment)(int)model[visuals::data::Properties::HORIZONTAL_ALIGNMENT]);
		}
		visuals::Fonts::WriteText(
				model[visuals::data::Properties::FONT],
				renderer, 
				common::XY<int>
				(
					model[common::data::Properties::X],
					model[common::data::Properties::Y]
				),
				model[visuals::data::Properties::TEXT],
				model[visuals::data::Properties::COLOR],
				(HorizontalAlignment)(int)model[visuals::data::Properties::HORIZONTAL_ALIGNMENT]);
	}
}
