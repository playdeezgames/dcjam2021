#pragma once
#include "Common.XY.h"
#include "Application.UIState.h"
#include <functional>
namespace application::MouseMotion
{
	typedef std::function<void(const common::XY<int>&)> Handler;
	void AddHandler(const ::UIState&, Handler);
	void Handle(const int&, const int&);
}
