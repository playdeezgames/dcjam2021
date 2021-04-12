#pragma once
#include <vector>
#include <optional>
#include <memory>
#include <SDL.h>
#include "Common.XY.h"
#include <string>
namespace graphics
{
	enum class HorizontalAlignment
	{
		RIGHT = -1,
		CENTER = 0,
		LEFT = 1
	};
}
namespace graphics::Fonts
{
	void InitializeFromFile(const std::string&);
	void WriteText(const std::string&, std::shared_ptr<SDL_Renderer>, const common::XY<int>&, const std::string& text, const std::string& color, const HorizontalAlignment& alignment);
}
