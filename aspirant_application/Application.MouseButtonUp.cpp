#include "Application.MouseButtonUp.h"
#include <map>
#include "Application.Handlers.h"
namespace application::MouseButtonUp
{
	static std::map<::UIState, Handler> handlers;

	void SetHandler(const ::UIState& state, Handler handler)
	{
		handlers[state] = handler;
	}

	void Handle(const SDL_MouseButtonEvent& evt)
	{
		application::Handlers::WithCurrent(
			handlers,
			[evt](const Handler& handler) { handler(common::XY<Sint32>(evt.x, evt.y), evt.button); });
	}
}
