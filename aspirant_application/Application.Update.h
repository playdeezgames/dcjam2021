#pragma once
#include <SDL.h>
#include "UIState.h"
#include <functional>
namespace application::Update
{
	typedef std::function<void(const Uint32&)> Handler;
	void AddHandler(const ::UIState&, Handler);
	void Handle(Uint32);
}