#include "Application.Update.h"
#include <map>
#include <vector>
#include "Application.Handlers.h"
namespace application::Update
{
	static std::map<::UIState, std::vector<Handler>> handlers;

	void AddHandler(const ::UIState& state, Handler handler)
	{
		if (handlers.find(state) == handlers.end())
		{
			handlers[state] = std::vector<Handler>();
		}
		handlers[state].push_back(handler);
	}

	void Handle(unsigned int ticks)
	{
		application::Handlers::WithCurrent(handlers, [ticks](const std::vector<Handler>& updaters) 
		{
			for (auto& updater : updaters)
			{
				updater(ticks);
			}
		});
	}


}