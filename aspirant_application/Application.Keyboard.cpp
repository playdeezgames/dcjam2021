#include <map>
#include "Command.h"
#include <SDL.h>
#include "json.hpp"
#include "Common.Utility.h"
#include "Data.JSON.h"
#include "Application.Keyboard.h"
namespace application::Keyboard
{
	static std::map<SDL_KeyCode, Command> keyboardCommands;

	static void InitializeKeyboardCommands(const nlohmann::json& config)
	{
		for (auto item : config.items())
		{
			SDL_KeyCode code = (SDL_KeyCode)common::Utility::StringToInt(item.key());
			Command command = (Command)(int)item.value();
			keyboardCommands[code] = command;
		}
	}

	std::optional<Command> ToCommand(SDL_KeyCode keycode)
	{
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

	void Start(const std::string& fileName)
	{
		auto properties = data::JSON::Load(fileName);
		InitializeKeyboardCommands(properties);
	}
}