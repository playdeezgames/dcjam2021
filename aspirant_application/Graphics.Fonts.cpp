#include "Graphics.Fonts.h"
#include "Data.JSON.h"
namespace graphics::Fonts
{
	static std::map<std::string, nlohmann::json> table;

	static void ParseDescriptor(const std::string& identifier, const nlohmann::json& properties)
	{
		table[identifier] = data::JSON::Load(properties);
	}

	std::optional<graphics::Font> Get(const std::string& key)
	{
		if (table.count(key) > 0)
		{
			return Font(table[key]);
		}
		else
		{
			return std::nullopt;
		}
	}

	void Start(const std::string& fileName)
	{
		auto properties = data::JSON::Load(fileName);
		for (auto& item : properties.items())
		{
			ParseDescriptor(item.key(), item.value());
		}
	}

}