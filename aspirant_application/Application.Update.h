#pragma once
#include "Application.UIState.h"
#include <functional>
namespace application::Update
{
	typedef std::function<void(const unsigned int&)> Handler;
	void AddHandler(const ::UIState&, Handler);
	void Handle(unsigned int);
}