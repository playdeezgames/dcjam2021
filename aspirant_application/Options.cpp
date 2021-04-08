#include "Options.h"
#include "Data.JSON.h"
#include "Common.Audio.h"
namespace Options
{
	const std::string MUTED = "muted";
	const std::string SFX_VOLUME = "sfxVolume";
	const std::string MUX_VOLUME = "muxVolume";
	static std::string fileName;

	void Start(const std::string& filename)
	{
		fileName = filename;
		auto properties = data::JSON::Load(fileName);
		common::Audio::SetMuted((bool)properties[MUTED]);
		common::audio::SetSfxVolume((int)properties[SFX_VOLUME]);
		common::audio::SetMuxVolume((int)properties[MUX_VOLUME]);
	}

	void Save()
	{
		nlohmann::json properties;
		properties[MUTED] = common::Audio::IsMuted();
		properties[MUX_VOLUME] = common::audio::GetMuxVolume();
		properties[SFX_VOLUME] = common::audio::GetSfxVolume();
		data::JSON::Save(fileName, properties);
	}
}