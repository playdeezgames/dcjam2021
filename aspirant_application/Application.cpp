#include <SDL.h>
#include <string>
#include <optional>
#include "Application.Renderer.h"
#include "Visuals.Colors.h"
#include "Visuals.Textures.h"
#include "Visuals.Sprites.h"
#include "Visuals.Fonts.h"
#include "Visuals.Layouts.h"
#include "Common.Audio.h"
#include "Options.h"
#include <map>
#include "Application.MouseMotion.h"
#include "Application.Command.h"
#include "Application.TextInput.h"
#include "Application.Update.h"
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
#include "Application.OnEnter.h"
#include "Game.Avatar.h"
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
		{ (Uint8)SDL_CONTROLLER_BUTTON_DPAD_DOWN,     Command::DOWN     },
		{ (Uint8)SDL_CONTROLLER_BUTTON_DPAD_UP,       Command::UP       },
		{ (Uint8)SDL_CONTROLLER_BUTTON_DPAD_LEFT,     Command::LEFT     },
		{ (Uint8)SDL_CONTROLLER_BUTTON_DPAD_RIGHT,    Command::RIGHT    },
		{ (Uint8)SDL_CONTROLLER_BUTTON_A,             Command::GREEN    },
		{ (Uint8)SDL_CONTROLLER_BUTTON_B,             Command::RED      },
		{ (Uint8)SDL_CONTROLLER_BUTTON_X,             Command::BLUE     },
		{ (Uint8)SDL_CONTROLLER_BUTTON_Y,             Command::YELLOW   },
		{ (Uint8)SDL_CONTROLLER_BUTTON_BACK,          Command::BACK     },
		{ (Uint8)SDL_CONTROLLER_BUTTON_START,         Command::START    },
		{ (Uint8)SDL_CONTROLLER_BUTTON_LEFTSHOULDER,  Command::PREVIOUS },
		{ (Uint8)SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, Command::NEXT     }
	};

	static void HandleControllerButtonDown(const SDL_ControllerButtonEvent& evt)
	{
		auto item = controllerButtonCommand.find(evt.button);
		if (item!=controllerButtonCommand.end())
		{
			::application::Command::Handle(item->second);
		}
	}
}
namespace common::Application
{
	static std::vector<std::function<void()>> starters = 
	{
		data::Stores::Start,
		state::Splash::Start,
		state::MainMenu::Start,
		state::About::Start,
		state::ConfirmQuit::Start,
		state::Options::Start,
		state::Start::Start,
		sublayout::UIHamburger::Start,
		sublayout::TabControls::Start,
		state::in_play::MiniMap::Start,
		state::LeavePlay::Start,
		sublayout::POV::Start,
		game::avatar::Statistics::Start,
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
		game::CombatDeck::Deal,
		game::CombatDeck::Deal,
		state::LoadGame::Start,
		state::SaveGame::Start,
		state::ConfirmAbandon::Start,
		Options::Initialize,
		visuals::Layouts::Start
	};

	void Start(std::shared_ptr<SDL_Renderer> renderer, const std::vector<std::string>& arguments)
	{
		for (auto& starter : starters)
		{
			starter();
		}
		common::audio::Mux::Play("songs");//TODO: nekkid string
	}

	bool IsRunning()
	{
		return ::application::UIState::Read() != ::UIState::QUIT;
	}

	static std::optional<::UIState> currentState = std::nullopt;

	void Update(Uint32 ticks)
	{
		if (!currentState || currentState.value() != ::application::UIState::Read())
		{
			currentState = ::application::UIState::Read();
			//load the layout!
			application::OnEnter::Handle();
		}
		application::Update::Handle(ticks);
	}

	std::map<Uint32, std::function<void(const SDL_Event&)>> eventHandlers =
	{
		{ SDL_QUIT,                 [](const SDL_Event&    ) { ::application::UIState::Write(::UIState::QUIT); }},
		{ SDL_KEYDOWN,              [](const SDL_Event& evt) { ::Application::HandleKeyDown(evt.key); }},
		{ SDL_TEXTINPUT,            [](const SDL_Event& evt) { ::application::TextInput::Handle(evt.text); }},
		{ SDL_CONTROLLERBUTTONDOWN, [](const SDL_Event& evt) { ::Application::HandleControllerButtonDown(evt.cbutton); }},
		{ SDL_MOUSEMOTION,          [](const SDL_Event& evt) { ::application::MouseMotion::Handle(evt.motion); }},
		{ SDL_MOUSEBUTTONUP,        [](const SDL_Event& evt) { ::application::MouseButtonUp::Handle(evt.button); }}
	};

	void HandleEvent(const SDL_Event& evt)
	{
		auto item = eventHandlers.find(evt.type);
		if (item!=eventHandlers.end())
		{
			item->second(evt);
		}
	}
}