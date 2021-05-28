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

	void Handle(const int& x, const int& y, const unsigned char& button)
	{
		application::Handlers::WithCurrent(
			mouseUpHandlers,
			[x,y,button](const std::vector<Handler>& handlers)
		{ 
			for (auto& handler : handlers)
			{
				if (handler(common::XY<int>(x, y), button))
				{
					return true;
				}
			}
			return false;
		});
	}
}
