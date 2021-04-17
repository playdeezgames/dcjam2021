#pragma once
#include <string>
#include <SDL.h>
#include <optional>
namespace graphics::Menus
{
	//TODO: this can perhaps replace avatarinventory and floorinventory if we make it generic enough
	std::optional<int> Read(const std::string&, const std::string&);
	void Write(const std::string&, const std::string&, int);
	size_t GetCount(const std::string&, const std::string&);
	void Next(const std::string&, const std::string&);
	void Previous(const std::string&, const std::string&);
}