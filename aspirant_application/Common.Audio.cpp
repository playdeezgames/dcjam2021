#include "Common.Audio.h"
#include <fstream>
#include "Common.Utility.h"
#include <fstream>
#include <memory>
#include <SDL_Mixer.h>
#include "Data.Stores.h"
namespace common::audio
{
	static std::map<std::string, std::shared_ptr<Mix_Chunk>> sounds;
	static int sfxVolume = MIX_MAX_VOLUME;

	static std::map<std::string, std::shared_ptr<Mix_Music>> music;
	static int muxVolume = MIX_MAX_VOLUME;

	const int ANY_CHANNEL = -1;
	const int NO_LOOPS = 0;
	const int LOOP_FOREVER = -1;

	static int ClampVolume(int volume)
	{
		return
			(volume < 0) ? (0) :
			(volume > MIX_MAX_VOLUME) ? (MIX_MAX_VOLUME) :
			(volume);
	}

	namespace Mux
	{
		static void HookFreeMusic(Mix_Music* m)
		{
			Mix_FreeMusic(m);
		}

		static void AddMusic(const std::string& name, const std::string& filename)
		{
			music[name] = std::shared_ptr<Mix_Music>(Mix_LoadMUS(filename.c_str()), HookFreeMusic);
		}

		static void Finish()
		{
			while (music.begin() != music.end())
			{
				music.erase(music.begin());
			}
		}

		static bool initialized = false;

		static void Initialize()
		{
			if (!initialized)
			{
				atexit(Finish);
				nlohmann::json& j = data::Stores::GetStore(data::Store::MUSIC_THEMES);
				for (auto& i : j.items())
				{
					Mux::AddMusic(i.key(), i.value());
				}
				initialized = true;
			}
		}

		const std::map<Theme, std::string> themeTable =
		{
			{Theme::MAIN, "main"},
			{Theme::BATTLE, "battle"}
		};

		static std::optional<Theme> currentTheme = std::nullopt;

		void Play(const Theme& theme)
		{
			Initialize();
			if (!common::Audio::IsMuted())
			{
				if (!currentTheme.has_value() || currentTheme.value() != theme)
				{
					currentTheme = theme;
					const auto& item = music.find(themeTable.find(theme)->second);
					Mix_FadeInMusic(item->second.get(), LOOP_FOREVER, 1000);
				}
			}
		}

		void SetVolume(int volume)
		{
			Initialize();
			muxVolume = ClampVolume(volume);
			Mix_VolumeMusic(muxVolume);
		}

		int GetVolume()
		{
			return muxVolume;
		}

		std::function<void()> GoToTheme(const Theme& theme)
		{
			return [theme]() { common::audio::Mux::Play(theme); };
		}
	}

	namespace Sfx
	{
		static void AddSound(const std::string& name, const std::string& filename)
		{
			sounds[name] = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV_RW(SDL_RWFromFile(filename.c_str(), "rb"), 1), Mix_FreeChunk);
		}

		static bool initialized = false;

		static void Initialize()
		{
			if (!initialized)
			{
				nlohmann::json& j = data::Stores::GetStore(data::Store::SOUND_EFFECTS);
				for (auto& i : j.items())
				{
					Sfx::AddSound(i.key(), i.value());
				}
				initialized = true;
			}
		}

		void Play(const std::string& name)
		{
			Initialize();
			if (!common::Audio::IsMuted())
			{
				const auto& item = sounds.find(name);
				if (item != sounds.end())
				{
					Mix_PlayChannel(ANY_CHANNEL, item->second.get(), NO_LOOPS);
				}
			}
		}

		void Play(const std::optional<std::string>& name)
		{
			if (name)
			{
				Play(name.value());
			}
		}

		void SetVolume(int volume)
		{
			Initialize();
			sfxVolume = ClampVolume(volume);
			for (auto& entry : sounds)
			{
				Mix_VolumeChunk(entry.second.get(), sfxVolume);
			}
		}

		int GetVolume()
		{
			return sfxVolume;
		}
	}

}
namespace common::Audio
{
	static bool muted = false;

	void Initialize()
	{
		common::audio::Sfx::Initialize();
		common::audio::Mux::Initialize();
	}

	void SetMuted(bool newValue)
	{
		Initialize();
		muted = newValue;
		if (muted)
		{
			Mix_PauseMusic();
		}
		else
		{
			Mix_ResumeMusic();
		}
	}

	bool IsMuted()
	{
		return muted;
	}
}

