#pragma once
#include "Application.UIState.h"
#include <functional>
namespace application::Update
{
	typedef std::function<void(const Uint32&)> Handler;
	void AddHandler(const ::UIState&, Handler);
	void Handle(Uint32);
}