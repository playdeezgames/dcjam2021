#pragma once
#include "Graphics.Font.h"
#include <vector>
#include <optional>
namespace graphics::Fonts
{
	void InitializeFromFile(const std::string&);
	std::optional<graphics::Font> Read(const std::string&);
}
