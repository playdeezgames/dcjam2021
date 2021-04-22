#include "Application.MouseMotion.h"
#include <map>
#include "Application.Handlers.h"
namespace application::MouseMotion
{
	static std::map<::UIState, std::vector<Handler>> mouseMotionHandlers;

	void Handle(const SDL_MouseMotionEvent& evt)
	{
		application::Handlers::WithCurrent(
			mouseMotionHandlers,
			[evt](const std::vector<Handler>& handlers)
		{ 
			for (auto& handler : handlers)
			{
				handler(common::XY<Sint32>(evt.x, evt.y));
			}
		});
	}

	void AddHandler(const ::UIState& state, Handler handler)
	{
		mouseMotionHandlers[state].push_back(handler);
	}
}