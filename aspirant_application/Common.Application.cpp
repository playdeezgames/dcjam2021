#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include "Data.JSON.h"
#include "Common.Data.Properties.h"
#include <memory>
#include "Common.Application.h"
namespace common::Application
{
	const std::string LOGICAL_WIDTH = "logicalWidth";
	const std::string LOGICAL_HEIGHT = "logicalHeight";
	const std::string TITLE = "title";
	const std::string ICON = "icon";
	const std::string MIXER_FREQUENCY = "mixerFrequency";
	const std::string CHANNEL_COUNT = "channelCount";
	const std::string CHUNK_SIZE = "chunkSize";

	static std::shared_ptr<SDL_Window> window = nullptr;
	static std::shared_ptr<SDL_Renderer> renderer = nullptr;

	extern void Start(std::shared_ptr<SDL_Renderer>, const std::vector<std::string>&);
	extern bool IsRunning();
	extern void Update(Uint32);
	extern void HandleEvent(const SDL_Event&);

	static std::map<int, SDL_GameController*> controllers;

	static void FinishControllers()
	{
		for (auto controller : controllers)
		{
			SDL_GameControllerClose(controller.second);
		}
		controllers.clear();
	}

	static void HookMixQuit()
	{
		Mix_Quit();
	}

	static void StartAudio(const nlohmann::json& properties)
	{
		int mixerFrequency = properties[MIXER_FREQUENCY];
		int channelCount = properties[CHANNEL_COUNT];
		int chunkSize = properties[CHUNK_SIZE];
		Mix_Init(MIX_INIT_OGG);
		Mix_OpenAudio
		(
			mixerFrequency,
			MIX_DEFAULT_FORMAT,
			channelCount,
			chunkSize
		);
		atexit(HookMixQuit);
		atexit(Mix_CloseAudio);
	}

	static void StartWindow(const nlohmann::json& properties)
	{
		int windowWidth = properties[common::data::Properties::WIDTH];
		int windowHeight = properties[common::data::Properties::HEIGHT];
		int logicalWidth = properties[LOGICAL_WIDTH];
		int logicalHeight = properties[LOGICAL_HEIGHT];
		std::string windowTitle = properties[TITLE];
		std::string iconFileName = properties[ICON];
		SDL_Window* pw = nullptr;
		SDL_Renderer* pr = nullptr;
		SDL_CreateWindowAndRenderer(
			windowWidth,
			windowHeight,
			0,
			&pw,
			&pr);
		window = std::shared_ptr<SDL_Window>(pw, SDL_DestroyWindow);
		renderer = std::shared_ptr<SDL_Renderer>(pr, SDL_DestroyRenderer);
		SDL_RenderSetLogicalSize(renderer.get(), logicalWidth, logicalHeight);
		SDL_SetWindowTitle(window.get(), windowTitle.c_str());
		auto iconSurface = IMG_Load(iconFileName.c_str());
		SDL_SetWindowIcon(window.get(), iconSurface);
		SDL_FreeSurface(iconSurface);
	}

	static void StartControllers()
	{
		atexit(FinishControllers);
		for (int index = 0; index < SDL_NumJoysticks(); ++index)
		{
			if (SDL_IsGameController(index))
			{
				controllers[index] = SDL_GameControllerOpen(index);
			}
		}
	}

	static void DoStart(const std::string& configFile, const std::vector<std::string>& arguments)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		atexit(SDL_Quit);

		auto properties = ::data::JSON::Load(configFile);
		StartAudio(properties);
		StartWindow(properties);
		StartControllers();
		Start(renderer, arguments);
	}

	static void DoPump()
	{
		SDL_Event evt;
		auto currentTicks = SDL_GetTicks();
		while (IsRunning())
		{
			auto frameTicks = SDL_GetTicks();
			Update(frameTicks - currentTicks);
			currentTicks = frameTicks;
			Render(renderer);
			SDL_RenderPresent(renderer.get());
			while (SDL_PollEvent(&evt))
			{
				HandleEvent(evt);
			}
		}
	}

	static void DoFinish()
	{
		Mix_CloseAudio();
		Mix_Quit();
	}

	int Run(const std::string& configFile, const std::vector<std::string>& arguments)
	{
		atexit(DoFinish);
		DoStart(configFile, arguments);
		DoPump();
		return 0;
	}
}
