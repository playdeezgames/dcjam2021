#include "Options.h"
#include "Data.JSON.h"
#include "Common.Sounds.h"
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
		common::Sounds::SetMuted((bool)properties[MUTED]);
		common::Sounds::SetSfxVolume((int)properties[SFX_VOLUME]);
		common::Sounds::SetMuxVolume((int)properties[MUX_VOLUME]);
	}

	void Save()
	{
		nlohmann::json properties;
		properties[MUTED] = common::Sounds::IsMuted();
		properties[MUX_VOLUME] = common::Sounds::GetMuxVolume();
		properties[SFX_VOLUME] = common::Sounds::GetSfxVolume();
		data::JSON::Save(fileName, properties);
	}
}