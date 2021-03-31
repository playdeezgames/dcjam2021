#include "Application.MouseWheel.h"
#include <SDL.h>
#include "Common.XY.h"
#include "UIState.h"
#include <map>
#include "Application.Handlers.h"
namespace application::MouseWheel
{
	static std::map<::UIState, Handler> handlers;

	void SetHandler(const ::UIState& state, Handler handler)
	{
		handlers[state] = handler;
	}

	void Handle(const SDL_MouseWheelEvent& evt)
	{
		application::Handlers::WithCurrent(
			handlers,
			[evt](const Handler& handler) { 
			handler(common::XY<Sint32>(
				(evt.direction == SDL_MOUSEWHEEL_FLIPPED) ? (-evt.x) : (evt.x),
				(evt.direction == SDL_MOUSEWHEEL_FLIPPED) ? (-evt.y) : (evt.y))); 
			});
	}
}
