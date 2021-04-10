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
namespace state::About { void Start(); }
namespace state::ConfirmQuit { void Start(); }
namespace state::MainMenu { void Start(); }
namespace state::Options { void Start(); }
namespace state::Splash { void Start(); }
namespace state::Start { void Start(); }
namespace state::in_play::MiniMap { void Start(); }
namespace state::LeavePlay { void Start(); }
namespace sublayout::POV { void Start(); }
namespace game::avatar::Items { void Start(); }
namespace state::in_play::FloorInventory { void Start(); }
namespace state::in_play::AvatarInventory { void Start(); }
namespace state::in_play::AvatarStatus { void Start(); }
namespace state::in_play::Combat { void Start(); }
namespace state::in_play::CombatResult { void Start(); }
namespace state::in_play::Dead { void Start(); }
namespace state::in_play::Exit { void Start(); }
namespace sublayout::QuickStats { void Start(); }
namespace sublayout::EnemyStats { void Start(); }
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
	const std::string TEXTURE = "config/graphics/textures.json";
	const std::string SPRITE = "config/graphics/sprites.json";
	const std::string COLOR = "config/graphics/colors.json";
	const std::string FONTS = "config/graphics/fonts.json";
	const std::string LAYOUTS = "config/ui/layouts.json";
	const std::string OPTIONS = "config/options.json";
	const std::string SFX = "config/audio/sfx.json";
	const std::string MUX = "config/audio/mux.json";
	const std::string KEYBOARD = "config/keyboard.json";
	const std::string CREATURES = "config/creatures.json";

	static std::vector<void(*)()> starters = 
	{
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
		game::CombatDeck::Deal,
		game::CombatDeck::Deal
	};

	void Start(std::shared_ptr<SDL_Renderer> renderer, const std::vector<std::string>& arguments)
	{
		graphics::Colors::Start(COLOR);
		graphics::Textures::InitializeFromFile(renderer, TEXTURE);
		graphics::Sprites::InitializeFromFile(SPRITE);
		graphics::Fonts::Start(FONTS);
		graphics::Layouts::Start(LAYOUTS);
		common::Audio::Start(SFX, MUX);
		Options::Start(OPTIONS);
		application::Keyboard::Start(KEYBOARD);
		game::Creatures::Start(CREATURES);

		for (auto starter : starters)
		{
			starter();
		}
		common::audio::PlayMusic("songs");
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