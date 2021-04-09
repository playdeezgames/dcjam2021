#pragma once
#include <string>
#include <SDL.h>
#include <optional>
namespace graphics::Menus
{
	//TODO: this can perhaps replace avatarinventory and floorinventory if we make it generic enough
	std::optional<int> Read(const std::string&, const std::string&);
	void Next(const std::string&, const std::string&);
	void Previous(const std::string&, const std::string&);
	//TODO: set font
	//TODO: set colors
	//TODO: set alignment
	//TODO: set drop shadow flag
	//TODO: set drop shadow color
	//TODO: set drop shadow xy
	//TODO: add menu item
	//TODO: remove menu item
	//TODO: set index
	//TODO: get item count
}