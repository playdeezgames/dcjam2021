#pragma once
#include <map>
#include <string>
#include <optional>
#include <functional>
namespace common::audio
{
	namespace Sfx
	{
		void Play(const std::string&);
		void Play(const std::optional<std::string>&);
		void SetVolume(int);
		int GetVolume();
	}

	namespace Mux
	{
		enum class Theme
		{
			MAIN,
			BATTLE
		};
		void Play(const Theme&);
		void SetVolume(int);
		int GetVolume();
		std::function<void()> GoToTheme(const Theme&);
	}
}
namespace common::Audio
{
	void SetMuted(bool);
	bool IsMuted();
}
