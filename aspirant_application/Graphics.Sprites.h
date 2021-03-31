#pragma once
#include <string>
#include "Graphics.Sprite.h"
#include <optional>
namespace graphics::Sprites
{
	std::optional<Sprite> Read(const std::string&);
	void InitializeFromFile(const std::string&);
}