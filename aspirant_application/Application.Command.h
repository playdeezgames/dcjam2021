#pragma once
#include "Application.UIState.h"
#include <functional>
#include <map>
enum class Command
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	GREEN,
	RED,
	YELLOW,
	BLUE,
	NEXT,
	PREVIOUS,
	BACK,
	START
};
namespace application::Command
{
	void SetHandlers(const ::UIState&, const std::map<::Command, std::function<void()>>&);
	void SetHandler(const ::UIState&, std::function<void()>);
	void Handle(const ::Command& command);
}
