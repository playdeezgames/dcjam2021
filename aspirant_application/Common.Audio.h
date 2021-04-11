#pragma once
#include <map>
#include <string>
#include <SDL_Mixer.h>
namespace common::audio
{

	void PlaySound(const std::string&);
	void SetSfxVolume(int);
	int GetSfxVolume();

	void PlayMusic(const std::string&);
	void SetMuxVolume(int);
	int GetMuxVolume();
}
namespace common::Audio
{
	void InitializeFromFile(const std::string&, const std::string&);
	void SetMuted(bool);
	bool IsMuted();
}
