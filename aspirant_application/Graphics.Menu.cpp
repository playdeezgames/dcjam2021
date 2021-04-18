#include "Graphics.Data.Properties.h"
#include "Graphics.Fonts.h"
#include "Common.Data.Properties.h"
#include "json.hpp"
namespace graphics::Menu
{
	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		int index = model[data::Properties::INDEX];
        std::string font = model[data::Properties::FONT];
		std::string activeColor = model[data::Properties::COLORS][data::Properties::ACTIVE];
		std::string inactiveColor = model[data::Properties::COLORS][data::Properties::INACTIVE];
		graphics::HorizontalAlignment horizontalAlignment = (graphics::HorizontalAlignment)(int)model[data::Properties::HORIZONTAL_ALIGNMENT];
		bool dropShadow = model[data::Properties::DROP_SHADOW];
		std::string dropShadowColor = model[data::Properties::DROP_SHADOW_COLOR];
		int dropShadowX = model[data::Properties::DROP_SHADOW_X];
		int dropShadowY = model[data::Properties::DROP_SHADOW_Y];
		auto& menuItems = model[data::Properties::MENU_ITEMS];
		int currentIndex = 0;
		for (auto& menuItem : menuItems)
		{
			std::string text = menuItem[data::Properties::TEXT];
			int x = menuItem[common::data::Properties::X];
			int y = menuItem[common::data::Properties::Y];
			if (dropShadow)
			{
				Fonts::WriteText(
					font,
					renderer, 
					common::XY<int>(x + dropShadowX, y + dropShadowY), 
					text, 
					dropShadowColor, 
					horizontalAlignment);
			}
			std::string color = (currentIndex == index) ? (activeColor) : (inactiveColor);
			Fonts::WriteText(font, renderer, common::XY<int>(x, y), text, color, horizontalAlignment);
			currentIndex++;
		}
	}
}