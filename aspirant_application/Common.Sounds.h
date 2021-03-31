#pragma once
#include <map>
#include <string>
#include <SDL_Mixer.h>
namespace common::Sounds
{
	void Start(const std::string&, const std::string&);

	void PlaySound(const std::string&);
	void SetSfxVolume(int);
	int GetSfxVolume();

	void PlayMusic(const std::string&);
	void SetMuxVolume(int);
	int GetMuxVolume();

	void SetMuted(bool);
	bool IsMuted();
}
