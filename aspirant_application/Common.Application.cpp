#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include "Data.JSON.h"
#include "Common.Properties.h"
namespace common::Application
{
	const std::string LOGICAL_WIDTH = "logicalWidth";
	const std::string LOGICAL_HEIGHT = "logicalHeight";
	const std::string TITLE = "title";
	const std::string ICON = "icon";
	const std::string MIXER_FREQUENCY = "mixerFrequency";
	const std::string CHANNEL_COUNT = "channelCount";
	const std::string CHUNK_SIZE = "chunkSize";

	static SDL_Window* window = nullptr;
	static SDL_Renderer* renderer = nullptr;

	extern void Start(SDL_Renderer*, const std::vector<std::string>&);
	extern bool IsRunning();
	extern void Update(Uint32);
	extern void Render(SDL_Renderer*);
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
		atexit(Mix_Quit);
		atexit(Mix_CloseAudio);
	}

	static void StartWindow(const nlohmann::json& properties)
	{
		int windowWidth = properties[common::Properties::WIDTH];
		int windowHeight = properties[common::Properties::HEIGHT];
		int logicalWidth = properties[LOGICAL_WIDTH];
		int logicalHeight = properties[LOGICAL_HEIGHT];
		std::string windowTitle = properties[TITLE];
		std::string iconFileName = properties[ICON];
		SDL_CreateWindowAndRenderer(
			windowWidth,
			windowHeight,
			0,
			&window,
			&renderer);
		SDL_RenderSetLogicalSize(renderer, logicalWidth, logicalHeight);
		SDL_SetWindowTitle(window, windowTitle.c_str());
		auto iconSurface = IMG_Load(iconFileName.c_str());
		SDL_SetWindowIcon(window, iconSurface);
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

		auto properties = data::JSON::Load(configFile);
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
			SDL_RenderPresent(renderer);
			while (SDL_PollEvent(&evt))
			{
				HandleEvent(evt);
			}
		}
	}

	template<typename TAsset>
	void CleanUpPointer(TAsset*& asset, void (*finisher)(TAsset*))
	{
		if (asset)
		{
			finisher(asset);
			asset = nullptr;
		}
	}

	static void DestroyWindow()
	{
		if (window)
		{
			SDL_DestroyWindow(window);
			window = nullptr;
		}
	}

	static void DoFinish()
	{
		CleanUpPointer(renderer, SDL_DestroyRenderer);
		CleanUpPointer(window, SDL_DestroyWindow);
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
