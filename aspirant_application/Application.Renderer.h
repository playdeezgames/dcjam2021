#pragma once
#include "UIState.h"
#include <string>
#include <SDL.h>
#include <memory>
namespace application::Renderer
{
	void SetRenderLayout(const ::UIState&, const std::string&);
}
