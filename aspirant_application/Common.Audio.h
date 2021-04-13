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
	void InitializeFromFile(const std::string&, const std::string&);
	void SetMuted(bool);
	bool IsMuted();
}
