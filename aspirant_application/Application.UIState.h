#pragma once
#include "UIState.h"
namespace application::UIState
{
	void Write(const ::UIState&);
	const ::UIState& Read();
}