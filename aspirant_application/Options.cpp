#include "Options.h"
#include "Data.JSON.h"
#include "Common.Audio.h"
namespace Options
{
	const std::string MUTED = "muted";
	const std::string SFX_VOLUME = "sfxVolume";
	const std::string MUX_VOLUME = "muxVolume";
	static std::string fileName;

	void InitializeFromFile(const std::string& filename)
	{
		fileName = filename;
		auto properties = data::JSON::Load(fileName);
		common::Audio::SetMuted((bool)properties[MUTED]);
		common::audio::Sfx::SetVolume((int)properties[SFX_VOLUME]);
		common::audio::Mux::SetVolume((int)properties[MUX_VOLUME]);
	}

	void Save()
	{
		nlohmann::json properties;
		properties[MUTED] = common::Audio::IsMuted();
		properties[MUX_VOLUME] = common::audio::Mux::GetVolume();
		properties[SFX_VOLUME] = common::audio::Sfx::GetVolume();
		data::JSON::Save(fileName, properties);
	}
}