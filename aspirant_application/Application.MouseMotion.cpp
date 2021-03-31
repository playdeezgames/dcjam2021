#include "Application.MouseMotion.h"
#include <map>
#include "Application.Handlers.h"
namespace application::MouseMotion
{
	static std::map<::UIState, Handler> handlers;

	void Handle(const SDL_MouseMotionEvent& evt)
	{
		application::Handlers::WithCurrent(
			handlers,
			[evt](const Handler& handler) { handler(common::XY<Sint32>(evt.x, evt.y)); });
	}

	void SetHandler(const ::UIState& state, Handler handler)
	{
		handlers[state] = handler;
	}
}