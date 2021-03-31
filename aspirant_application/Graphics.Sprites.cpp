#include "Graphics.Sprites.h"
#include "Common.Utility.h"
#include "Data.JSON.h"
#include "Graphics.Textures.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
namespace graphics::Sprites
{
	static std::map<std::string, Sprite> sprites;
	static nlohmann::json table;

	std::optional<Sprite> Read(const std::string& name)
	{
		if (table.count(name) > 0)
		{
			return Sprite(table[name]);
		}
		else
		{
			return std::nullopt;
		}
	}

	void InitializeFromFile(const std::string& fileName)
	{
		table = data::JSON::Load(fileName);
	}
}
