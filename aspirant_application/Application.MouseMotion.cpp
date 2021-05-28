#include "Application.MouseMotion.h"
#include <map>
#include "Application.Handlers.h"
namespace application::MouseMotion
{
	static std::map<::UIState, std::vector<Handler>> mouseMotionHandlers;

	void Handle(const int& x, const int& y)
	{
		application::Handlers::WithCurrent(
			mouseMotionHandlers,
			[x,y](const std::vector<Handler>& handlers)
		{ 
			for (auto& handler : handlers)
			{
				handler(common::XY<int>(x, y));
			}
		});
	}

	void AddHandler(const ::UIState& state, Handler handler)
	{
		mouseMotionHandlers[state].push_back(handler);
	}
}