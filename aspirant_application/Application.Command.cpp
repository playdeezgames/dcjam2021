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
}