#include "Application.TextInput.h"
#include <map>
#include "Application.Handlers.h"
namespace application::TextInput
{
	static std::map<::UIState, Handler> handlers;

	void Handle(const SDL_TextInputEvent& evt)
	{
		application::Handlers::WithCurrent(
			handlers, 
			[evt](const Handler& handler) { handler(evt.text); });
	}

	void SetHandler(const ::UIState& state, Handler handler)
	{
		handlers[state] = handler;
	}
}