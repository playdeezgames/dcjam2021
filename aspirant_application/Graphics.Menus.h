#pragma once
#include <string>
#include <SDL.h>
#include <optional>
namespace graphics::Menus
{
	std::optional<int> ReadIndex(const std::string&, const std::string&);
	std::optional<int> ReadValue(const std::string&, const std::string&);
	void WriteIndex(const std::string&, const std::string&, int);
	bool WriteValue(const std::string&, const std::string&, int);
	size_t GetCount(const std::string&, const std::string&);
	void Next(const std::string&, const std::string&);
	void Previous(const std::string&, const std::string&);
}