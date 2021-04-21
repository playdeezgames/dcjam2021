#include "Application.OnEnter.h"
#include <map>
#include <vector>
#include "Application.Handlers.h"
namespace application::OnEnter
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

	void Handle()
	{
		application::Handlers::WithCurrent(handlers, [](const std::vector<Handler>& enterers)
		{
			for (auto& enterer : enterers)
			{
				enterer();
			}
		});
	}
}