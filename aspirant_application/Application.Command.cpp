#include "Application.Command.h"
#include "Application.UIState.h"
#include <map>
#include "Application.Handlers.h"
#include "Common.Utility.h"
namespace application::Command
{
	static std::map<::UIState, std::map<::Command, std::function<void()>>> commandHandlers;

	void Handle(const ::Command& command)
	{
		application::Handlers::WithCurrent(commandHandlers, 
			[command](const std::map<::Command, std::function<void()>>& handlers) 
			{
				common::Utility::Dispatch(handlers, command);
			});
	}

	void SetHandlers(const ::UIState& state, const std::map<::Command, std::function<void()>>& handlers)
	{
		commandHandlers[state] = handlers;
	}

	const std::vector<::Command> commands =
	{
		::Command::UP,
		::Command::DOWN,
		::Command::LEFT,
		::Command::RIGHT,
		::Command::GREEN,
		::Command::RED,
		::Command::YELLOW,
		::Command::BLUE,
		::Command::NEXT,
		::Command::PREVIOUS,
		::Command::BACK,
		::Command::START
	};

	void SetHandler(const ::UIState& state, std::function<void()> handler)
	{
		std::map<::Command, std::function<void()>> commandHandlers;
		for (auto command : commands)
		{
			commandHandlers[command] = handler;
		}
		SetHandlers(state, commandHandlers);
	}

}