#pragma once
#include "json.hpp"
namespace data
{
	enum class Store
	{
		CREATURE_DESCRIPTORS,
		ITEM_DESCRIPTORS,
		COLORS,
		SOUND_EFFECTS,
		MUSIC_THEMES,
		TEXTURES,
		SPRITES,
		FONTS,
		LAYOUTS,
		KEYS,
		OPTIONS,
		STATISTICS
	};
}
namespace data::Stores
{
	nlohmann::json& GetStore(const data::Store&);
	void Start();
	void Save(const data::Store&);
}
