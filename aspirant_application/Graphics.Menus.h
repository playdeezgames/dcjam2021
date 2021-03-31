#pragma once
#include <string>
#include <SDL.h>
#include <optional>
namespace graphics::Menus
{
	std::optional<int> Read(const std::string&, const std::string&);
	void Next(const std::string&, const std::string&);
	void Previous(const std::string&, const std::string&);
}