#include "Data.Stores.h"
#include <map>
#include <string>
#include "Data.JSON.h"
namespace data::Stores
{
	static std::map<data::Store, nlohmann::json> stores;

	nlohmann::json& GetStore(const data::Store& store)
	{
		return stores[store];
	}

	const std::map<data::Store, std::string> storeFiles =
	{
		{data::Store::CREATURE_DESCRIPTORS, "config/creatures.json"},
		{data::Store::ITEM_DESCRIPTORS, "config/items.json"},
		{data::Store::COLORS, "config/graphics/colors.json" },
		{data::Store::SOUND_EFFECTS, "config/audio/sfx.json"},
		{data::Store::MUSIC_THEMES, "config/audio/mux.json"},
		{data::Store::TEXTURES, "config/graphics/textures.json"},
		{data::Store::SPRITES, "config/graphics/sprites.json"},
		{data::Store::FONTS, "config/graphics/fonts.json"},
		{data::Store::LAYOUTS, "config/ui/layouts.json"},
		{data::Store::KEYS, "config/keyboard.json"},
		{data::Store::OPTIONS, "config/options.json"},
		{data::Store::STATISTICS, "config/statistics.json"},
		{data::Store::UI_SFX, "config/ui/sfx.json"},
		{data::Store::AVATAR, "config/avatar.json"},
		{data::Store::MAZE, "config/maze.json"}
	};

	void Start()
	{
		for (auto& entry : storeFiles)
		{
			stores[entry.first] = ::data::JSON::Load(entry.second);
		}
	}

	void Save(const data::Store& store)
	{
		auto iter = storeFiles.find(store);
		if (iter != storeFiles.end())
		{
			data::JSON::Save(iter->second, GetStore(store));
		}
	}

}
