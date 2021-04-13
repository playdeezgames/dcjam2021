#include "Common.Audio.h"
#include <fstream>
#include "Common.Utility.h"
#include <fstream>
#include "Data.JSON.h"
namespace common::audio
{
	static std::map<std::string, Mix_Chunk*> sounds;
	static int sfxVolume = MIX_MAX_VOLUME;

	static std::map<std::string, Mix_Music*> music;
	static int muxVolume = MIX_MAX_VOLUME;

	const int ANY_CHANNEL = -1;
	const int NO_LOOPS = 0;
	const int LOOP_FOREVER = -1;

	static int ClampVolume(int volume)
	{
		return
			(volume < 0) ? (0) :
			(volume > SDL_MIX_MAXVOLUME) ? (SDL_MIX_MAXVOLUME) :
			(volume);

	}

	namespace Mux
	{
		static void FinishMusic()
		{
			Mix_HaltMusic();
			for (auto& entry : music)
			{
				if (entry.second)
				{
					Mix_FreeMusic(entry.second);
					entry.second = nullptr;
				}
			}
			music.clear();
		}
		static void AddMusic(const std::string& name, const std::string& filename)
		{
			music[name] = Mix_LoadMUS(filename.c_str());
		}
		void Play(const std::string& name)
		{
			if (!common::Audio::IsMuted())
			{
				const auto& item = music.find(name);
				Mix_PlayMusic(item->second, LOOP_FOREVER);
			}
		}

		void SetVolume(int volume)
		{
			muxVolume = ClampVolume(volume);
			Mix_VolumeMusic(muxVolume);
		}

		int GetVolume()
		{
			return muxVolume;
		}


	}

	namespace Sfx
	{
		static void FinishSound()
		{
			for (auto& entry : sounds)
			{
				if (entry.second)
				{
					Mix_FreeChunk(entry.second);
					entry.second = nullptr;
				}
			}
			sounds.clear();
		}

		static void AddSound(const std::string& name, const std::string& filename)
		{
			sounds[name] = Mix_LoadWAV(filename.c_str());
		}

		void Play(const std::string& name)
		{
			if (!common::Audio::IsMuted())
			{
				const auto& item = sounds.find(name);
				if (item != sounds.end())
				{
					Mix_PlayChannel(ANY_CHANNEL, item->second, NO_LOOPS);
				}
			}
		}

		void SetVolume(int volume)
		{
			sfxVolume = ClampVolume(volume);
			for (auto& entry : sounds)
			{
				Mix_VolumeChunk(entry.second, sfxVolume);
			}
		}
		int GetVolume()
		{
			return sfxVolume;
		}



	}

	static void Finish()
	{
		Mux::FinishMusic();
		Sfx::FinishSound();
	}


	static void StartSound(const std::string& sfxFileName)
	{
		nlohmann::json j = data::JSON::Load(sfxFileName);
		for (auto& i : j.items())
		{
			Sfx::AddSound(i.key(), i.value());
		}
	}

	static void StartMusic(const std::string& muxFileName)
	{
		nlohmann::json j = data::JSON::Load(muxFileName);
		for (auto& i : j.items())
		{
			Mux::AddMusic(i.key(), i.value());
		}
	}
}
namespace common::Audio
{
	static bool muted = false;

	void SetMuted(bool newValue)
	{
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

	void InitializeFromFile(const std::string& sfxFileName, const std::string& muxFileName)
	{
		atexit(common::audio::Finish);
		common::audio::StartSound(sfxFileName);
		common::audio::StartMusic(muxFileName);
	}
}

