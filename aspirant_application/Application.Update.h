#pragma once
#include <SDL.h>
#include "UIState.h"
namespace application::Update
{
	typedef void (*Handler)(const Uint32&);
	void AddHandler(const UIState&, Handler);
	void Handle(Uint32);
}