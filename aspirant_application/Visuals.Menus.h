#pragma once
#include <string>
#include <optional>
#include <functional>
namespace visuals::Menus
{
	std::optional<int> ReadIndex(const std::string&, const std::string&);
	void WriteIndex(const std::string&, const std::string&, int);
	size_t GetCount(const std::string&, const std::string&);
	std::function<void()> NavigateNext(const std::string&, const std::string&);
	std::function<void()> NavigatePrevious(const std::string&, const std::string&);
}
namespace visuals::MenuItems
{
	void SetText(const std::string&, const std::string&, const std::string&);
	void SetEnabled(const std::string&, const std::string&, bool);
}