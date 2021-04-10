#include "Graphics.Properties.h"
#include "Graphics.Fonts.h"
#include "Common.Properties.h"
#include "json.hpp"
namespace graphics::Menu
{
	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		int index = model[Properties::INDEX];
        std::string font = model[Properties::FONT];
		std::string activeColor = model[Properties::COLORS][Properties::ACTIVE];
		std::string inactiveColor = model[Properties::COLORS][Properties::INACTIVE];
		graphics::HorizontalAlignment horizontalAlignment = (graphics::HorizontalAlignment)(int)model[Properties::HORIZONTAL_ALIGNMENT];
		bool dropShadow = model[Properties::DROP_SHADOW];
		std::string dropShadowColor = model[Properties::DROP_SHADOW_COLOR];
		int dropShadowX = model[Properties::DROP_SHADOW_X];
		int dropShadowY = model[Properties::DROP_SHADOW_Y];
		auto& menuItems = model[Properties::MENU_ITEMS];
		int currentIndex = 0;
		for (auto& menuItem : menuItems)
		{
			std::string text = menuItem[Properties::TEXT];
			int x = menuItem[common::Properties::X];
			int y = menuItem[common::Properties::Y];
			if (dropShadow)
			{
				Fonts::Read(font).value().WriteText(
					renderer, 
					common::XY<int>(x + dropShadowX, y + dropShadowY), 
					text, 
					dropShadowColor, 
					horizontalAlignment);
			}
			std::string color = (currentIndex == index) ? (activeColor) : (inactiveColor);
			Fonts::Read(font).value().WriteText(renderer, common::XY<int>(x, y), text, color, horizontalAlignment);
			currentIndex++;
		}
	}
}