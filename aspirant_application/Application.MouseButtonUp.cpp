#include "Application.MouseButtonUp.h"
#include <map>
#include "Application.Handlers.h"
namespace application::MouseButtonUp
{
	static std::map<::UIState, std::vector<Handler>> mouseUpHandlers;

	void AddHandler(const ::UIState& state, Handler handler)
	{
		mouseUpHandlers[state].push_back(handler);
	}

	void Handle(const SDL_MouseButtonEvent& evt)
	{
		application::Handlers::WithCurrent(
			mouseUpHandlers,
			[evt](const std::vector<Handler>& handlers)
		{ 
			for (auto& handler : handlers)
			{
				if (handler(common::XY<Sint32>(evt.x, evt.y), evt.button))
				{
					return true;
				}
			}
			return false;
		});
	}
}
