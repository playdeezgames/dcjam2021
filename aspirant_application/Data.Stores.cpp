#include "Data.Stores.h"
#include <map>
#include <string>
#include "Data.JSON.h"
#include "Common.Utility.h"
#include <optional>
namespace data::Stores
{
	static std::map<data::Store, nlohmann::json> stores;
	static bool modded = false;

	nlohmann::json& GetStore(const data::Store& store)
	{
		return stores[store];
	}

	struct storeFile
	{
		std::string filename;
		std::optional<unsigned char> checkSum;
	};

	const std::map<data::Store, storeFile> storeFiles =
	{
		{data::Store::CREATURE_DESCRIPTORS, {"config/creatures.json",237}},
		{data::Store::ITEM_DESCRIPTORS, {"config/items.json",163}},
		{data::Store::COLORS, {"config/graphics/colors.json",std::nullopt}},
		{data::Store::SOUND_EFFECTS, {"config/audio/sfx.json",std::nullopt}},
		{data::Store::MUSIC_THEMES, {"config/audio/mux.json",std::nullopt}},
		{data::Store::TEXTURES, {"config/graphics/textures.json",std::nullopt}},
		{data::Store::SPRITES, {"config/graphics/sprites.json",std::nullopt}},
		{data::Store::FONTS, {"config/graphics/fonts.json",std::nullopt}},
		{data::Store::LAYOUTS, {"config/ui/layouts.json",std::nullopt}},
		{data::Store::KEYS, {"config/keyboard.json",std::nullopt}},
		{data::Store::OPTIONS, {"config/options.json",std::nullopt}},
		{data::Store::STATISTICS, {"config/statistics.json",17}},
		{data::Store::UI_SFX, {"config/ui/sfx.json",std::nullopt}},
		{data::Store::AVATAR, {"config/avatar.json",194}},
		{data::Store::MAZE, {"config/maze.json",104}},
		{data::Store::SHOPPES, {"config/shoppes.json",70}}
	};

	void Start()
	{
		for (auto& entry : storeFiles)
		{
			auto checkSum = common::Utility::GetFileCheckSum(entry.second.filename);
			if (entry.second.checkSum.has_value() && checkSum != entry.second.checkSum.value())
			{
				modded = true;
#ifdef _DEBUG
				throw entry.second.filename;
#endif
			}
			stores[entry.first] = ::data::JSON::Load(entry.second.filename);
		}
	}

	void Save(const data::Store& store)
	{
		auto iter = storeFiles.find(store);
		if (iter != storeFiles.end())
		{
			data::JSON::Save(iter->second.filename, GetStore(store));
		}
	}

	bool IsModded()
	{
		return modded;
	}
}
