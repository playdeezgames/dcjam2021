#include "Application.Command.h"
#include "UIState.h"
#include <map>
#include "Application.Handlers.h"
namespace application::Command
{
	static std::map<::UIState, Handler> handlers;

	void Handle(const ::Command& command)
	{
		application::Handlers::WithCurrent(handlers, [command](const Handler& handler) {handler(command); });
	}

	void SetHandler(const ::UIState& state, Handler handler)
	{
		handlers[state] = handler;
	}

	static std::map<::UIState, std::map<::Command, std::function<void()>>> commandHandlers;

	void SetHandlers(const ::UIState& state, const std::map<::Command, std::function<void()>>& handlers)
	{
		commandHandlers[state] = handlers;
	}
}