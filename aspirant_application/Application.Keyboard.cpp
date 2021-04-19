#include <map>
#include "Command.h"
#include <SDL.h>
#include "json.hpp"
#include "Common.Utility.h"
#include "Data.JSON.h"
#include "Application.Keyboard.h"
#include "Data.Stores.h"
#pragma warning (disable: 26812)
namespace application::Keyboard
{
	static std::map<SDL_KeyCode, Command> keyboardCommands;

	static bool initialized = false;

	static void Initialize()
	{
		if (!initialized)
		{
			auto& config = data::Stores::GetStore(data::Store::KEYS);
			for (auto& item : config.items())
			{
				SDL_KeyCode code = (SDL_KeyCode)common::Utility::StringToInt(item.key());
				Command command = (Command)(int)item.value();
				keyboardCommands[code] = command;
			}
			initialized = true;
		}
	}

	std::optional<Command> ToCommand(SDL_KeyCode keycode)
	{
		Initialize();
		auto iter = keyboardCommands.find(keycode);
		if (iter != keyboardCommands.end())
		{
			return iter->second;
		}
		else
		{
			return std::nullopt;
		}
	}
}