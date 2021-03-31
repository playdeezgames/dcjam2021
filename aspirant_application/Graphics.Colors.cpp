#include "Graphics.Colors.h"
#include "Data.JSON.h"
#include <map>
#include <vector>
#include "Graphics.Properties.h"
namespace graphics::Colors
{
	static nlohmann::json table;
	
	static SDL_Color ParseDescriptor(const nlohmann::json& properties)
	{
		SDL_Color result;
		result.r = (Uint8)properties[Properties::RED];
		result.g = (Uint8)properties[Properties::GREEN];
		result.b = (Uint8)properties[Properties::BLUE];
		result.a = (Uint8)properties[Properties::ALPHA];
		return result;
	}

	void Start(const std::string& fileName)
	{
		table = data::JSON::Load(fileName);
	}

	SDL_Color Get(const std::string& key)
	{
		return ParseDescriptor(table[key]);
	}
}