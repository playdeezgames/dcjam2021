#include <SDL.h>
#include <string>
#include <optional>
#include "UIState.h"
#include "Command.h"
#include "Application.Renderer.h"
#include "Graphics.Colors.h"
#include "Graphics.Textures.h"
#include "Graphics.Sprites.h"
#include "Graphics.Fonts.h"
#include "Graphics.Layouts.h"
#include "Common.Audio.h"
#include "Options.h"
#include <map>
#include "Application.MouseMotion.h"
#include "Application.Command.h"
#include "Application.TextInput.h"
#include "Application.Update.h"
#include "Application.MouseWheel.h"
#include "Application.MouseButtonDown.h"
#include "Application.MouseButtonUp.h"
#include "Application.UIState.h"
#include "Game.h"
#include "Common.Utility.h"
#include "Application.Keyboard.h"
#include "Game.Creatures.h"
#include "Game.Combat.h"
#include "Game.CombatDeck.h"
#include "Game.Item.h"
#include "Game.Avatar.Items.h"
#include "States.h"
#include "Sublayouts.h"
#include "Game.Avatar.Statistics.h"
#include "Data.Stores.h"
namespace Application
{
	static std::optional<::Command> KeyCodeToCommand(SDL_KeyCode code)
	{
		return application::Keyboard::ToCommand(code);
	}

	static void HandleKeyDown(const SDL_KeyboardEvent& evt)
	{
		auto command = application::Keyboard::ToCommand((SDL_KeyCode)evt.keysym.sym);
		if (command)
		{
			application::Command::Handle(command.value());
		}
	}

	static std::map<Uint8, Command> controllerButtonCommand = 
	{
		{(Uint8)SDL_CONTROLLER_BUTTON_DPAD_DOWN, Command::DOWN},
		{(Uint8)SDL_CONTROLLER_BUTTON_DPAD_UP, Command::UP},
		{(Uint8)SDL_CONTROLLER_BUTTON_DPAD_LEFT, Command::LEFT},
		{(Uint8)SDL_CONTROLLER_BUTTON_DPAD_RIGHT, Command::RIGHT},
		{(Uint8)SDL_CONTROLLER_BUTTON_A, Command::GREEN },
		{(Uint8)SDL_CONTROLLER_BUTTON_B, Command::RED},
		{(Uint8)SDL_CONTROLLER_BUTTON_X, Command::BLUE},
		{(Uint8)SDL_CONTROLLER_BUTTON_Y, Command::YELLOW},
		{(Uint8)SDL_CONTROLLER_BUTTON_BACK, Command::BACK},
		{(Uint8)SDL_CONTROLLER_BUTTON_START, Command::START},
		{(Uint8)SDL_CONTROLLER_BUTTON_LEFTSHOULDER, Command::PREVIOUS},
		{(Uint8)SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, Command::NEXT}
	};

	static void HandleControllerButtonDown(const SDL_ControllerButtonEvent& evt)
	{
		auto iter = controllerButtonCommand.find(evt.button);
		if (iter != controllerButtonCommand.end())
		{
			::application::Command::Handle(iter->second);
		}
	}

}
namespace common::Application
{
	const std::string OPTIONS = "config/options.json";
	const std::string KEYBOARD = "config/keyboard.json";
	const std::string STATISTICS = "config/statistics.json";

	static std::vector<void(*)()> starters = 
	{
		data::Stores::Start,
		state::Splash::Start,
		state::MainMenu::Start,
		state::About::Start,
		state::ConfirmQuit::Start,
		state::Options::Start,
		state::Start::Start,
		state::in_play::MiniMap::Start,
		state::LeavePlay::Start,
		sublayout::POV::Start,
		game::avatar::Items::Start,
		state::in_play::FloorInventory::Start,
		state::in_play::AvatarInventory::Start,
		state::in_play::AvatarStatus::Start,
		sublayout::QuickStats::Start,
		sublayout::EnemyStats::Start,
		game::Start,
		state::in_play::Combat::Start,
		state::in_play::CombatResult::Start,
		state::in_play::Dead::Start,
		state::in_play::Exit::Start,
		game::CombatDeck::Deal
	};

	void Start(std::shared_ptr<SDL_Renderer> renderer, const std::vector<std::string>& arguments)
	{
		application::Keyboard::InitializeFromFile(KEYBOARD);
		game::avatar::Statistics::InitializeFromFile(STATISTICS);

		for (auto starter : starters)
		{
			starter();
		}
		Options::InitializeFromFile(OPTIONS);//TODO: this may have to stay here and not be part of the data store
		common::audio::Mux::Play("songs");//TODO: nekkid string
	}

	bool IsRunning()
	{
		return ::application::UIState::Read() != ::UIState::QUIT;
	}

	void Update(Uint32 ticks)
	{
		application::Update::Handle(ticks);
	}

	void HandleEvent(const SDL_Event& evt)
	{
		switch (evt.type)
		{
		case SDL_QUIT:
			::application::UIState::Write(::UIState::QUIT);
			break;
		case SDL_KEYDOWN:
			::Application::HandleKeyDown(evt.key);
			break;
		case SDL_TEXTINPUT:
			::application::TextInput::Handle(evt.text);
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			::Application::HandleControllerButtonDown(evt.cbutton);
			break;
		case SDL_MOUSEMOTION:
			::application::MouseMotion::Handle(evt.motion);
			break;
		case SDL_MOUSEBUTTONDOWN:
			::application::MouseButtonDown::Handle(evt.button);
			break;
		case SDL_MOUSEBUTTONUP:
			::application::MouseButtonUp::Handle(evt.button);
			break;
		case SDL_MOUSEWHEEL:
			::application::MouseWheel::Handle(evt.wheel);
			break;
		}
	}
}