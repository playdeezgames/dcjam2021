#pragma once
#include <string>
#include <SDL.h>
#include <optional>
#include <functional>
namespace visuals::Menus
{
	std::optional<int> ReadIndex(const std::string&, const std::string&);
	std::optional<int> ReadValue(const std::string&, const std::string&);
	void WriteIndex(const std::string&, const std::string&, int);
	bool WriteValue(const std::string&, const std::string&, int);
	size_t GetCount(const std::string&, const std::string&);
	std::function<void()> NavigateNext(const std::string&, const std::string&);
	std::function<void()> NavigatePrevious(const std::string&, const std::string&);
}
namespace visuals::MenuItems
{
	void SetText(const std::string&, const std::string&, const std::string&);
}