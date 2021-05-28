#pragma once
#include "Application.UIState.h"
#include <functional>
namespace application::OnEnter
{
	typedef std::function<void()> Handler;
	void AddHandler(const ::UIState&, Handler);
	void Handle();
}