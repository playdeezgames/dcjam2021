#pragma once
#include "Common.XY.h"
#include "Application.UIState.h"
#include <functional>
namespace application::MouseButtonUp
{
	typedef std::function<bool(const common::XY<int>&, unsigned char)> Handler;
	void AddHandler(const ::UIState&, Handler);
	void Handle(const int&, const int&, const unsigned char&);
}
