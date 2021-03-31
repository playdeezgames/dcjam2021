#pragma once
#include "Graphics.Font.h"
#include <vector>
#include <optional>
namespace graphics::Fonts
{
	void Start(const std::string&);
	std::optional<graphics::Font> Get(const std::string&);
}
