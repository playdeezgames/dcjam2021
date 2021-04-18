#pragma once
#include <map>
#include <string>
namespace common::audio
{
	namespace Sfx
	{
		void Play(const std::string&);
		void SetVolume(int);
		int GetVolume();
	}

	namespace Mux
	{
		void Play(const std::string&);
		void SetVolume(int);
		int GetVolume();
	}
}
namespace common::Audio
{
	void SetMuted(bool);
	bool IsMuted();
}
